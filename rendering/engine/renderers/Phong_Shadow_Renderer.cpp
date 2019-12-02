#include "Phong_Shadow_Renderer.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

#include "Defs.h"

//for testing
#include "IO/Image.h"
#include <iostream>

using namespace glgpu;
using namespace math;
using namespace world;

namespace rndr
{
	Phong_Shadow_Renderer
	phong_shadow_create()
	{
		Phong_Shadow_Renderer self{};

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self.depth_prog = program_create("../rendering/engine/shaders/depth.vertex", "../rendering/engine/shaders/depth.pixel");
		self.phong_shadow_prog = program_create("../rendering/engine/shaders/phong_shadow.vertex", "../rendering/engine/shaders/phong_shadow.pixel");
		self.fb = framebuffer_create();
		self.depth = texture2d_create(SHADOW_WIDTH, SHADOW_HEIGHT, INTERNAL_TEXTURE_FORMAT::RGBA, TEXTURE_FORMAT::RGBA, DATA_TYPE::UBYTE);

		return self;
	}

	void
	phong_shadow_free(Phong_Shadow_Renderer & mr)
	{
		program_delete(mr.depth_prog);
		program_delete(mr.phong_shadow_prog);
		framebuffer_free(mr.fb);
		texture_free(mr.depth);
	}

	void
	phong_shadow_pack(Phong_Shadow_Renderer& mr, const world::object3D* mesh)
	{
		mr.meshes.push_back(mesh);
	}

	void
	phong_shadow_unpack(Phong_Shadow_Renderer & mr)
	{
		mr.meshes.clear();
	}

	void
	phong_shadow_draw(const Phong_Shadow_Renderer& mr, const vec3f& light_pos, const world::Camera& viewer)
	{
		//calc shadow map (depth buffer from light prespective) -- for now we render to color framebuffer color attachment as depth attachment does not work right (TODO)
		{
			//clear depth buffer
			depth_clear();

			//viewport
			view_port(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

			//bind framebuffer (render color buffer to color tex for testing)
			framebuffer_bind(mr.fb);
			texture2d_bind(mr.depth, TEXTURE_UNIT::UNIT_0);
			framebuffer_attach(mr.fb, mr.depth, FRAMEBUFFER_ATTACHMENT::COLOR0);
			//disable_color_buffer_rw();

			//render scene
			program_use(mr.depth_prog);
			for (const auto object : mr.meshes)
			{
				//MVP
				Mat4f mvp = /*light_view_proj*/ camera_view_proj(camera_new()) * mat4_from_transform(object->model);
				uniformmat4f_set(mr.depth_prog, "mvp", mvp);

				//draw geometry
				vao_bind(object->mesh.va, object->mesh.vs, object->mesh.is);
				draw_indexed(object->mesh.indices.size());
				vao_unbind();
			}
			framebuffer_unbind();
			texture2d_unbind();

			//testing the output texture
			{
				io::Image img{ SHADOW_WIDTH, SHADOW_HEIGHT, 4 };
				img.data = new unsigned char[SHADOW_WIDTH * SHADOW_HEIGHT * 4];
				texture2d_unpack(mr.depth, img, TEXTURE_FORMAT::RGBA, DATA_TYPE::UBYTE);

				/*unsigned int* dep = (unsigned int*)img.data;
				for (int x = 0; x < 4 * img.width * img.height; x += 4)
				{
					if (*dep !=0 && *dep != 4294967040)
						std::cout << *dep << std::endl;
					dep++;
				}*/

				io::image_write(img, "../rendering/IO/shadow_map.jpg", io::FORMAT::JPG);
				io::image_free(img);
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