#include "Depth_Offline_Renderer.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

using namespace glgpu;
using namespace math;
using namespace world;

namespace rndr
{
	Depth_Offline_Renderer
	depth_offline_create()
	{
		Depth_Offline_Renderer self{};

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self.prog = program_create("../rendering/engine/shaders/depth.vertex", "../rendering/engine/shaders/depth.pixel");
		self.fb = framebuffer_create();
		self.depth = texture2d_create(1024, 1024, TEXTURE_FORMAT::DEPTH, DATA_TYPE::FLOAT);
		return self;
	}

	void
	depth_offline_free(Depth_Offline_Renderer & mr)
	{
		program_delete(mr.prog);
		framebuffer_free(mr.fb);
		texture_free(mr.depth);
	}

	void
	depth_offline_pack(Depth_Offline_Renderer& mr, const world::object3D* mesh)
	{
		mr.meshes.push_back(mesh);
	}

	void
	depth_offline_unpack(Depth_Offline_Renderer & mr)
	{
		mr.meshes.clear();
	}

	texture
	depth_offline_draw(const Depth_Offline_Renderer& mr, const math::Mat4f& view_proj)
	{
		frame_start();
		program_use(mr.prog);

		framebuffer_bind(mr.fb);
		framebuffer_attach(mr.fb, mr.depth, FRAMEBUFFER_ATTACHMENT::DEPTH);
		disable_color_buffer_rw();

		//render scene
		{
			//viewport
			view_port(0, 0, 1024, 1024);

			for (const auto object : mr.meshes)
			{
				//MVP
				Mat4f mvp = view_proj * mat4_from_transform(object->model);
				uniformmat4f_set(mr.prog, "mvp", mvp);

				//draw geometry
				vao_bind(object->mesh.va, object->mesh.vs, object->mesh.is);
				draw_indexed(object->mesh.indices.size());
				vao_unbind();
			}
		}
		framebuffer_unbind();

		return mr.depth;
	}
}