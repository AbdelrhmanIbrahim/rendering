#include "Phong_Shadow.h"

#include <math/Vector.h>
#include <math/Matrix.h>
#include <math/Gfx.h>

#include "Defs/Defs.h"

#include "gpu_gl/glgpu.h"

#include <vector>

using namespace glgpu;
using namespace math;
using namespace world;

namespace rndr
{
	struct IPhong_Shadow
	{
		glgpu::Framebuffer fb;
		glgpu::Texture depth;
		glgpu::Program depth_prog;
		glgpu::Program phong_shadow_prog;
		std::vector<const world::object3D*> meshes;
	};

	Phong_Shadow
	phong_shadow_create()
	{
		IPhong_Shadow* self = new IPhong_Shadow;

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self->depth_prog = program_create("F:/Abdo/rendering_jo/rendering/engine/shaders/depth.vertex", "F:/Abdo/rendering_jo/rendering/engine/shaders/depth.pixel");
		self->phong_shadow_prog = program_create("F:/Abdo/rendering_jo/rendering/engine/shaders/phong_shadow.vertex", "F:/Abdo/rendering_jo/rendering/engine/shaders/phong_shadow.pixel");
		self->fb = framebuffer_create();
		self->depth = texture2d_create(vec2f{ SHADOW_WIDTH, SHADOW_HEIGHT}, INTERNAL_TEXTURE_FORMAT::DEPTH_STENCIL, EXTERNAL_TEXTURE_FORMAT::DEPTH_STENCIL, DATA_TYPE::UINT_24_8, false);

		return self;
	}

	void
	phong_shadow_free(Phong_Shadow self)
	{
		program_delete(self->depth_prog);
		program_delete(self->phong_shadow_prog);
		framebuffer_free(self->fb);
		texture_free(self->depth);

		delete self;
	}

	void
	phong_shadow_pack(Phong_Shadow self, const world::object3D* mesh)
	{
		self->meshes.push_back(mesh);
	}

	void
	phong_shadow_unpack(Phong_Shadow self)
	{
		self->meshes.clear();
	}

	void
	phong_shadow_draw(const Phong_Shadow self, const vec3f& light_pos, const world::Camera& viewer)
	{
		//calc shadow map (depth buffer from light prespective) -- for now we render to color framebuffer color attachment as depth attachment does not work right (TODO)
		{
			//viewport
			view_port(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

			//bind framebuffer (render color buffer to color tex for testing)
			framebuffer_bind(self->fb);
			texture2d_bind(self->depth, TEXTURE_UNIT::UNIT_0);
			framebuffer_attach(self->fb, self->depth, FRAMEBUFFER_ATTACHMENT::DEPTH_STENCIL);
			
			//clear depth buffer
			depth_clear();
			disable_color_buffer_rw();

			//render scene
			program_use(self->depth_prog);
			for (const auto object : self->meshes)
			{
				//MVP
				Mat4f mvp = /*light_view_proj*/ camera_view_proj(camera_new()) * mat4_from_transform(object->model);
				uniformmat4f_set(self->depth_prog, "mvp", mvp);

				//draw geometry
				vao_bind(object->mesh.va, object->mesh.vs, object->mesh.is);
				draw_indexed(object->mesh.indices.size());
				vao_unbind();
			}
			framebuffer_unbind();
			texture2d_unbind();

			//testing the output texture
			{
				/*io::Image img{ SHADOW_WIDTH, SHADOW_HEIGHT, 4 };
				img.data = new unsigned char[SHADOW_WIDTH * SHADOW_HEIGHT * 4];
				texture2d_unpack(mr.depth, img, TEXTURE_FORMAT::DEPTH_STENCIL, DATA_TYPE::UINT_24_8);
				io::image_write(img, "../IO/shadow_map.jpg", io::IMAGE_FORMAT::JPG);
				io::image_free(img);*/
			}
		}

		//now do phong lighting but with the shadow map to calc shadows
		{

			//depth_clear();
			//program_use(mr.phong_shadow_prog);

			////viewport
			//vec2f viewport = world::camera_viewport(viewer);
			//view_port(0, 0, (int)viewport[0], (int)viewport[1]);

			////unifroms
			//uniform3f_set(mr.phong_shadow_prog, "light_color", vec3f{ 1.0f, 1.0f, 1.0f });
			//uniform3f_set(mr.phong_shadow_prog, "light_dir", vec3f{ 0.0f, -1.0f, 0.0f });
			//uniform3f_set(mr.phong_shadow_prog, "camera_world_pos", viewer.pos);
			//
			////shadow map
			//texture2d_bind(mr.depth, TEXTURE_UNIT::UNIT_0);
			//uniform1i_set(mr.phong_shadow_prog, "shadow_map", TEXTURE_UNIT::UNIT_0);

			//for (const auto object : mr.meshes)
			//{
			//	//MVP
			//	Mat4f model = mat4_from_transform(object->model);
			//	Mat4f mvp = camera_view_proj(viewer) * model;

			//	uniformmat4f_set(mr.phong_shadow_prog, "mvp", mvp);
			//	uniformmat4f_set(mr.phong_shadow_prog, "model", model);
			//	uniform3f_set(mr.phong_shadow_prog, "object_color", vec3f{ 1.0, 0.5, 0.31 });

			//	//draw geometry
			//	vao_bind(object->mesh.va, object->mesh.vs, object->mesh.is);
			//	draw_indexed(object->mesh.indices.size());
			//	vao_unbind();
			//}
		}
	}
}