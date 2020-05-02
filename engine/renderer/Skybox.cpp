#include "engine/renderer/Skybox.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

#include "world/component/Vertex.h"
#include "world/component/Camera.h"

#include "gl/glgpu.h"

#include "IO/Image.h"

using namespace glgpu;
using namespace math;
using namespace world;
using namespace io;

namespace rndr
{
	struct ISkybox
	{
		glgpu::Program prog;
		glgpu::Vao cube_vao;
		glgpu::Buffer cube_vbo;
		glgpu::Buffer uniform_space;
		glgpu::Cubemap cubemap;
		glgpu::Sampler sampler;
	};

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

	struct Space_Uniform
	{
		Mat4f view;
		Mat4f proj;
	};

	void
	_skybox_init(Skybox self)
	{
		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self->prog = program_create(DIR_PATH"/engine/shaders/skybox.vertex", DIR_PATH"/engine/shaders/skybox.pixel");
		self->cube_vbo = buffer_vertex_create();
		self->cube_vao = vao_create();
		vao_attach(self->cube_vao, self->cube_vbo);
		buffer_vertex_set(self->cube_vbo, skybox,	  sizeof(skybox), STORAGE::STATIC);
		buffer_vertex_attribute(self->cube_vbo, 0, 3, sizeof(world::Vertex), 0);
		buffer_vertex_attribute(self->cube_vbo, 1, 3, sizeof(world::Vertex), 3 * sizeof(float));
		buffer_vertex_attribute(self->cube_vbo, 2, 2, sizeof(world::Vertex), 6 * sizeof(float));
		vao_unbind();
	}

	Skybox
	skybox_renderer_rgba_create(const char** skybox_paths, IMAGE_FORMAT format)
	{
		ISkybox* self = new ISkybox;
		_skybox_init(self);

		//load skybox faces
		io::Image imgs[6];
		for (int i = 0; i < 6; ++i)
			imgs[i] = image_read(skybox_paths[i], format);
		self->cubemap = cubemap_rgba_create(imgs);
		self->sampler = sampler_create(TEXTURE_FILTERING::LINEAR, TEXTURE_FILTERING::LINEAR, TEXTURE_SAMPLING::CLAMP_TO_EDGE);

		for (int i = 0; i < 6; ++i)
			image_free(imgs[i]);

		return self;
	}

	Skybox
	skybox_renderer_hdr_create(const char* skybox_hdr_path)
	{
		ISkybox* self = new ISkybox;
		_skybox_init(self);

		//load skybox hdr
		Image img = image_read(skybox_hdr_path, IMAGE_FORMAT::HDR);
		self->cubemap = cubemap_hdr_create(img, vec2f{800, 800}, false);
		self->sampler = sampler_create(TEXTURE_FILTERING::LINEAR, TEXTURE_FILTERING::LINEAR, TEXTURE_SAMPLING::CLAMP_TO_EDGE);
		self->uniform_space = buffer_uniform_create(sizeof(Space_Uniform));

		image_free(img);

		return self;
	}

	void
	skybox_renderer_free(Skybox self)
	{
		program_delete(self->prog);
		cubemap_free(self->cubemap);
		vao_delete(self->cube_vao);
		buffer_delete(self->cube_vbo);
		buffer_delete(self->uniform_space);
		sampler_free(self->sampler);

		delete self;
	}

	void
	skybox_renderer_draw(const Skybox self, const Camera* cam)
	{
		depth_test(DEPTH_TEST::LE);
		{
			program_use(self->prog);

			//Uniform blocks
			buffer_uniform_bind(0, self->uniform_space);
			Space_Uniform mvp{ camera_view_matrix(*cam), camera_proj_matrix(*cam)};
			buffer_uniform_set(self->uniform_space, &mvp, sizeof(mvp));

			//viewport
			vec2f viewport = camera_viewport(*cam);
			view_port(0, 0, (int)viewport[0], (int)viewport[1]);

			//cubemap, till we get sampler objects in
			cubemap_bind(self->cubemap, 0);
			sampler_bind(self->sampler, 0);

			//draw world cube
			vao_bind(self->cube_vao);
			draw_strips(36);
			vao_unbind();
		}
		depth_test(DEPTH_TEST::L);
	}
}