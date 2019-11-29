#include "Skybox_Renderer.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

#include "geometry/vertex.h"

using namespace glgpu;
using namespace math;
using namespace world;
using namespace geo;

namespace rndr
{
	constexpr static Vertex skybox[36] = 
	{
		Vertex{-1.0f,  1.0f, -1.0f},
		Vertex{-1.0f, -1.0f, -1.0f},
		Vertex{ 1.0f, -1.0f, -1.0f},
		Vertex{ 1.0f, -1.0f, -1.0f},
		Vertex{ 1.0f,  1.0f, -1.0f},
		Vertex{-1.0f,  1.0f, -1.0f},

		Vertex{-1.0f, -1.0f,  1.0f},
		Vertex{-1.0f, -1.0f, -1.0f},
		Vertex{-1.0f,  1.0f, -1.0f},
		Vertex{-1.0f,  1.0f, -1.0f},
		Vertex{-1.0f,  1.0f,  1.0f},
		Vertex{-1.0f, -1.0f,  1.0f},

		Vertex{ 1.0f, -1.0f, -1.0f},
		Vertex{ 1.0f, -1.0f,  1.0f},
		Vertex{ 1.0f,  1.0f,  1.0f},
		Vertex{ 1.0f,  1.0f,  1.0f},
		Vertex{ 1.0f,  1.0f, -1.0f},
		Vertex{ 1.0f, -1.0f, -1.0f},

		Vertex{-1.0f, -1.0f,  1.0f},
		Vertex{-1.0f,  1.0f,  1.0f},
		Vertex{ 1.0f,  1.0f,  1.0f},
		Vertex{ 1.0f,  1.0f,  1.0f},
		Vertex{ 1.0f, -1.0f,  1.0f},
		Vertex{-1.0f, -1.0f,  1.0f},

		Vertex{-1.0f,  1.0f, -1.0f},
		Vertex{ 1.0f,  1.0f, -1.0f},
		Vertex{ 1.0f,  1.0f,  1.0f},
		Vertex{ 1.0f,  1.0f,  1.0f},
		Vertex{-1.0f,  1.0f,  1.0f},
		Vertex{-1.0f,  1.0f, -1.0f},

		Vertex{-1.0f, -1.0f, -1.0f},
		Vertex{-1.0f, -1.0f,  1.0f},
		Vertex{ 1.0f, -1.0f, -1.0f},
		Vertex{ 1.0f, -1.0f, -1.0f},
		Vertex{-1.0f, -1.0f,  1.0f},
		Vertex{ 1.0f, -1.0f,  1.0f}
	};

	Skybox_Renderer
	skybox_renderer_create(const char** skybox_paths)
	{
		Skybox_Renderer self{};

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self.prog = program_create("../rendering/engine/shaders/skybox.vertex", "../rendering/engine/shaders/skybox.pixel");
		self.cube = vao_create();
		self.cube_vs = vertex_buffer_create(skybox, 36);
		self.cubemap = cubemap_create(skybox_paths);
		return self;
	}

	void
	skybox_renderer_free(const Skybox_Renderer & sbr)
	{
		program_delete(sbr.prog);
		vao_delete(sbr.cube);
		buffer_delete(sbr.cube_vs);
		texture_free(sbr.cubemap);
	}

	void
	skybox_renderer_draw(const Skybox_Renderer& sbr, const Camera& cam)
	{
		depth_test(DEPTH_TEST::LE);
		{
			program_use(sbr.prog);

			//MVP
			Mat4f view = camera_view_matrix(cam);
			Mat4f proj = camera_proj_matrix(cam);
			uniformmat4f_set(sbr.prog, "view", view);
			uniformmat4f_set(sbr.prog, "proj", proj);

			//viewport
			vec2f viewport = camera_viewport(cam);
			view_port(0, 0, (int)viewport[0], (int)viewport[1]);

			//cubemap
			cubemap_bind(sbr.cubemap, TEXTURE_UNIT::UNIT_0);
			uniform1i_set(sbr.prog, "cubemap", TEXTURE_UNIT::UNIT_0);

			//draw world cube
			vao_bind(sbr.cube, sbr.cube_vs, NULL);
			draw_strip(36);
			vao_unbind();
		}
		depth_test(DEPTH_TEST::L);
	}
}