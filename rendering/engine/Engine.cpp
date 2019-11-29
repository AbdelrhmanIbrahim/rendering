#include "Engine.h"

#include "world/World.h"
#include "world/Camera.h"

#include "engine/renderers/Phong_Renderer.h"
#include "engine/renderers/PBR_Renderer.h"
#include "engine/renderers/Skybox_Renderer.h"

//nope (refactor later --revisit--)
#include "gpu_gl/glgpu.h"

using namespace world;

namespace rndr
{
	struct Engine
	{
		Phong_Renderer phong;
		PBR_Renderer pbr;
		Skybox_Renderer skybox;
	};

	Engine*
	engine_create()
	{
		Engine* self = new Engine;
		
		static const char* skybox_paths[6]
		{
			"../rendering/res/imgs/skybox/right.jpg",
			"../rendering/res/imgs/skybox/left.jpg",
			"../rendering/res/imgs/skybox/top.jpg",
			"../rendering/res/imgs/skybox/bottom.jpg",
			"../rendering/res/imgs/skybox/front.jpg",
			"../rendering/res/imgs/skybox/back.jpg"
		};

		self->skybox = skybox_renderer_create(skybox_paths);
		self->phong = phong_renderer_create();
		self->pbr = pbr_renderer_create();

		return self;
	}

	void
	engine_free(Engine* e)
	{
		phong_renderer_free(e->phong);
		pbr_renderer_free(e->pbr);
		skybox_renderer_free(e->skybox);

		delete e;
	}

	void
	engine_world_draw(const Engine* e, World* w, const math::vec2f& viewport)
	{
		//configure the camera
		camera_viewport(w->cam, viewport);

		//enable tests and clear color and depth buffers (refactor later)
		glgpu::frame_start();

		//render all meshes in the world using engine mesh renderer (add a buffer of meshes to each renderer -- revisit)
		pbr_renderer_draw(e->pbr, w->obj, w->cam);
		//phong_renderer_draw(e->phong, w->obj, w->cam);
		skybox_renderer_draw(e->skybox, w->cam);
	}
};