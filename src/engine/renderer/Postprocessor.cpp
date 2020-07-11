#include "Postprocessor.h"

#include "defs/Defs.h"

#include <vector>

using namespace glgpu;

namespace rndr
{
    struct IPass
    {
        glgpu::Program prog;
        std::vector<glgpu::Uniform> uniforms;
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
    
    Postprocessor
	postprocessor_create()
    {
        IPostprocessor* self = new IPostprocessor;
        self->palette_vao = vao_create();
        self->palette_vbo = buffer_vertex_create();
		self->uvp = buffer_uniform_create(sizeof(math::Mat4f));
        self->fb = framebuffer_create();
        self->out = texture2d_create(math::vec2f{1,1}, INTERNAL_TEXTURE_FORMAT::RGBA, EXTERNAL_TEXTURE_FORMAT::RGBA, DATA_TYPE::UBYTE, false);

        //palette quad
		vao_attach(self->palette_vao, self->palette_vbo);
		buffer_vertex_attribute(self->palette_vbo, 0, 3, sizeof(world::TVertex), 0);
		buffer_vertex_attribute(self->palette_vbo, 1, 3, sizeof(world::TVertex), sizeof(world::TVertex::pos));
		buffer_vertex_attribute(self->palette_vbo, 2, 2, sizeof(world::TVertex), sizeof(world::TVertex::pos) + sizeof(world::TVertex::normal));
		buffer_vertex_set(self->palette_vbo, tfr_quad, 6 * sizeof(world::TVertex), STORAGE::STATIC);

        //postprocess vertex shader uniforms
        math::Mat4f identity = math::mat4_id();
		buffer_uniform_bind(0, self->uvp);
		buffer_uniform_set(self->uvp, &identity, sizeof(math::Mat4f));

        return self;
    }

    void
    postprocessor_free(Postprocessor self)
    {
        vao_delete(self->palette_vao);
        buffer_delete(self->palette_vbo);
		buffer_delete(self->uvp);
        framebuffer_free(self->fb);
        texture_free(self->out);

        for(auto pass : self->passes)
            program_delete(pass.prog);
    }

    void
    postprocessor_effect_add(Postprocessor self, Pass& pass)
    {
        self->passes.push_back
        (
            IPass
            {
                program_create(DIR_PATH"/src/engine/shaders/tquad.vertex", pass.frag_shader_path),
                pass.uniforms
            }
        );
    }

	glgpu::Texture
    postprocessor_run(Postprocessor self, math::vec2f viewport)
    {
        view_port(0,0, viewport[0], viewport[1]);
        if(viewport != texture2d_size(self->out))
            texture2d_resize(self->out, viewport, INTERNAL_TEXTURE_FORMAT::RGBA, EXTERNAL_TEXTURE_FORMAT::RGBA, DATA_TYPE::UBYTE);

        framebuffer_bind(self->fb);
        framebuffer_attach(self->fb, self->out, FRAMEBUFFER_ATTACHMENT::COLOR0);
		{
            program_use(self->passes.front().prog);
            color_clear(1, 1, 1, 1);
            depth_clear();
        	depth_test(DEPTH_TEST::LE);
            vao_bind(self->palette_vao);
		    draw_strips(6);
		    vao_unbind();
        }
        framebuffer_unbind();

        return self->out;
    }
};