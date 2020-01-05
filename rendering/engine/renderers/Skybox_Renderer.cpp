#include "Skybox_Renderer.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

#include "geometry/vertex.h"

using namespace glgpu;
using namespace math;
using namespace world;
using namespace geo;
using namespace io;

namespace rndr
{
	constexpr static Vertex skybox[36] = 
	{
		//front
		Vertex{-1.0f, -1.0f,  1.0f},
		Vertex{-1.0f,  1.0f,  1.0f},
		Vertex{ 1.0f,  1.0f,  1.0f},
		Vertex{ 1.0f,  1.0f,  1.0f},
		Vertex{ 1.0f, -1.0f,  1.0f},
		Vertex{-1.0f, -1.0f,  1.0f},

		//back
		Vertex{-1.0f,  1.0f, -1.0f},
		Vertex{-1.0f, -1.0f, -1.0f},
		Vertex{ 1.0f, -1.0f, -1.0f},
		Vertex{ 1.0f, -1.0f, -1.0f},
		Vertex{ 1.0f,  1.0f, -1.0f},
		Vertex{-1.0f,  1.0f, -1.0f},

		//right
		Vertex{ 1.0f, -1.0f, -1.0f},
		Vertex{ 1.0f, -1.0f,  1.0f},
		Vertex{ 1.0f,  1.0f,  1.0f},
		Vertex{ 1.0f,  1.0f,  1.0f},
		Vertex{ 1.0f,  1.0f, -1.0f},
		Vertex{ 1.0f, -1.0f, -1.0f},

		//left
		Vertex{-1.0f, -1.0f,  1.0f},
		Vertex{-1.0f, -1.0f, -1.0f},
		Vertex{-1.0f,  1.0f, -1.0f},
		Vertex{-1.0f,  1.0f, -1.0f},
		Vertex{-1.0f,  1.0f,  1.0f},
		Vertex{-1.0f, -1.0f,  1.0f},

		//top
		Vertex{-1.0f,  1.0f, -1.0f},
		Vertex{ 1.0f,  1.0f, -1.0f},
		Vertex{ 1.0f,  1.0f,  1.0f},
		Vertex{ 1.0f,  1.0f,  1.0f},
		Vertex{-1.0f,  1.0f,  1.0f},
		Vertex{-1.0f,  1.0f, -1.0f},

		//bottom
		Vertex{-1.0f, -1.0f, -1.0f},
		Vertex{-1.0f, -1.0f,  1.0f},
		Vertex{ 1.0f, -1.0f, -1.0f},
		Vertex{ 1.0f, -1.0f, -1.0f},
		Vertex{-1.0f, -1.0f,  1.0f},
		Vertex{ 1.0f, -1.0f,  1.0f}
	};

	void
	skybox_init(Skybox_Renderer& self)
	{
		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self.prog = program_create("../rendering/engine/shaders/skybox.vertex", "../rendering/engine/shaders/skybox.pixel");
		self.cube = vao_create();
		self.cube_vs = vertex_buffer_create(skybox, 36);
	}

	Skybox_Renderer
	skybox_renderer_rgba_create(const char** skybox_paths, io::IMAGE_FORMAT format)
	{
		Skybox_Renderer self{};
		skybox_init(self);

		//load skybox faces
		io::Image imgs[6];
		for (int i = 0; i < 6; ++i)
			imgs[i] = image_read(skybox_paths[i], format);
		self.cubemap = cubemap_rgba_create(imgs);
		for (int i = 0; i < 6; ++i)
			image_free(imgs[i]);

		return self;
	}

	Skybox_Renderer
	skybox_renderer_hdr_create(const char* skybox_hdr_path)
	{
		Skybox_Renderer self{};
		skybox_init(self);

		//load skybox hdr
		Image img = image_read(skybox_hdr_path, io::IMAGE_FORMAT::HDR);
		self.cubemap = cubemap_hdr_create(img);

		image_free(img);

		return self;
	}

	void
	skybox_renderer_free(const Skybox_Renderer & sbr)
	{
		program_delete(sbr.prog);
		vao_delete(sbr.cube);
		buffer_delete(sbr.cube_vs);
		cubemap_free(sbr.cubemap);
	}

	void
	skybox_renderer_draw(const Skybox_Renderer& sbr, const Camera& cam)
	{
		auto mesh = geo::mesh_create("../rendering/res/stls/cube.stl");
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

			//for testing on small cube
			/*auto mesh = geo::mesh_create("../rendering/res/stls/cube.stl");
			vao_bind(mesh.va, mesh.vs, mesh.is);
			draw_indexed(mesh.indices.size());
			vao_unbind();
			mesh_delete(mesh);*/
			
		}
		depth_test(DEPTH_TEST::L);
	}
}