#include "Postprocessor.h"

#include "defs/Defs.h"

#include "gl/glgpu.h"

using namespace glgpu;

namespace rndr
{
    struct CGPU_Uniform
    {
        glgpu::Uniform ucpu;
        glgpu::IGL_Handle* ugpu;
    };

    struct IPass
    {
        glgpu::Program prog;
        std::vector<CGPU_Uniform> gpu_uniforms;
    };

    struct IPostprocessor
    {
       glgpu::Vao palette_vao;
       glgpu::Buffer palette_vbo;
       glgpu::Buffer uvp;
       glgpu::Framebuffer fb;
       glgpu::Texture out;

       std::vector<IPass> passes;
    };

    //internals
    CGPU_Uniform
    _cpu_to_gpu_uniform(Uniform uniform)
    {
        switch (uniform.type)
        {
            case UNIFORM_TYPE::FLOAT:
            {
                glgpu::Buffer ufloat = buffer_uniform_create(sizeof(float));
                buffer_uniform_set(ufloat, &uniform.value.f, sizeof(float));
                return CGPU_Uniform {uniform, ufloat};
            }
            case UNIFORM_TYPE::VEC3:
            {
                glgpu::Buffer uvec3 = buffer_uniform_create(sizeof(math::Vec3f));
                buffer_uniform_set(uvec3, &uniform.value.vec3, sizeof(math::Vec3f));
               return CGPU_Uniform {uniform, uvec3};
            }
            case UNIFORM_TYPE::VEC4:
            {
                glgpu::Buffer uvec4 = buffer_uniform_create(sizeof(math::Vec4f));
                buffer_uniform_set(uvec4, &uniform.value.vec4, sizeof(math::Vec4f));
                return CGPU_Uniform {uniform, uvec4};
            }
            case UNIFORM_TYPE::MAT4:
            {
                glgpu::Buffer umat4 = buffer_uniform_create(sizeof(math::Mat4f));
                buffer_uniform_set(umat4, &uniform.value.mat4, sizeof(math::Mat4f));
                return CGPU_Uniform {uniform, umat4};
            }
            case UNIFORM_TYPE::TEXTURE2D:
            {
                glgpu::Sampler usampler = sampler_create(TEXTURE_FILTERING::LINEAR, TEXTURE_FILTERING::LINEAR, TEXTURE_SAMPLING::CLAMP_TO_EDGE);
                 return CGPU_Uniform {uniform, usampler};
            }
            default:
                return CGPU_Uniform{};
        }
    }

    std::vector<CGPU_Uniform>
    _cpu_to_gpu_uniforms(std::vector<Uniform>& cpu_uniforms)
    {
        std::vector<CGPU_Uniform> gpu_uniforms;

        for(const auto& uniform : cpu_uniforms)
            gpu_uniforms.emplace_back(_cpu_to_gpu_uniform(uniform));

        return gpu_uniforms;
    }

    void
    _bind_gpu_uniform(const CGPU_Uniform& cgpu_uniform)
    {
        switch (cgpu_uniform.ucpu.type)
        {
            case UNIFORM_TYPE::FLOAT:
            case UNIFORM_TYPE::VEC3:
            case UNIFORM_TYPE::VEC4:
            case UNIFORM_TYPE::MAT4:
            {
                buffer_uniform_bind(cgpu_uniform.ugpu, cgpu_uniform.ucpu.slot);
                break;
            }
            case UNIFORM_TYPE::TEXTURE2D:
            {
            	sampler_bind(cgpu_uniform.ugpu, cgpu_uniform.ucpu.slot);
		        texture2d_bind(cgpu_uniform.ucpu.value.texture, cgpu_uniform.ucpu.slot);
                break;
            }
            default:
                break;
        }
    }

    //API
    Postprocessor
	postprocessor_create()
    {
        IPostprocessor* self = new IPostprocessor;
        self->palette_vao = vao_create();
        self->palette_vbo = buffer_vertex_create();
		self->uvp = buffer_uniform_create(sizeof(math::Mat4f));
        self->fb = framebuffer_create();
        self->out = texture2d_create(math::Vec2f{1,1}, INTERNAL_TEXTURE_FORMAT::RGBA, EXTERNAL_TEXTURE_FORMAT::RGBA, DATA_TYPE::UBYTE, false);

        //palette quad
		vao_attach(self->palette_vao, self->palette_vbo);
		buffer_vertex_attribute(self->palette_vbo, 0, 3, sizeof(world::TVertex), 0);
		buffer_vertex_attribute(self->palette_vbo, 1, 3, sizeof(world::TVertex), sizeof(world::TVertex::pos));
		buffer_vertex_attribute(self->palette_vbo, 2, 2, sizeof(world::TVertex), sizeof(world::TVertex::pos) + sizeof(world::TVertex::normal));
		buffer_vertex_set(self->palette_vbo, tfr_quad, 6 * sizeof(world::TVertex), STORAGE::STATIC);

        //postprocess vertex shader uniforms
        math::Mat4f identity = math::mat4_id();
		buffer_uniform_bind(self->uvp, 0);
		buffer_uniform_set(self->uvp, &identity, sizeof(math::Mat4f));

        return self;
    }

    void
    postprocessor_free(Postprocessor self)
    {
        handle_free(self->palette_vao);
        handle_free(self->palette_vbo);
        handle_free(self->uvp);
        handle_free(self->fb);
        handle_free(self->out);

        //delete passes
        for(auto& pass : self->passes)
        {
            handle_free(pass.prog);
            for(auto& uniform : pass.gpu_uniforms)
                handle_free(uniform.ugpu);
        }
    }

    void
    postprocessor_effect_add(Postprocessor self, rndr::Postprocessing_Pass& pass)
    {
        self->passes.emplace_back
        (
            IPass
            {
                program_create(DIR_PATH"/src/engine/shaders/tquad.vertex", pass.frag_shader_path),
                _cpu_to_gpu_uniforms(pass.cpu_uniforms)
            }
        );
    }

	glgpu::Texture
    postprocessor_run(Postprocessor self, math::Vec2f viewport)
    {
        view_port(0,0, viewport[0], viewport[1]);
        if(viewport != texture2d_size(self->out))
            texture2d_resize(self->out, viewport, INTERNAL_TEXTURE_FORMAT::RGBA, EXTERNAL_TEXTURE_FORMAT::RGBA, DATA_TYPE::UBYTE);

        framebuffer_bind(self->fb);
        framebuffer_attach(self->fb, self->out, FRAMEBUFFER_ATTACHMENT::COLOR0);
		{
            //run all your passes here
            for(const auto& pass : self->passes)
            {
                //prepare postprocessor
                {
                    program_use(pass.prog);
                    for(const auto& uniform : pass.gpu_uniforms)
                        _bind_gpu_uniform(uniform);
                }

                //postprocess
                {
                    color_clear(1, 1, 1, 1);
                    depth_clear();
                    depth_test(DEPTH_TEST::LE);
                    vao_bind(self->palette_vao);
                    draw_strips(6);
                    vao_unbind();
                }
            }
        }
        framebuffer_unbind();

        return self->out;
    }
};