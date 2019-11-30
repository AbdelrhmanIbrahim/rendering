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
		
		//skybox
		/*
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
		*/

		self->phong = phong_create();
		self->pbr = pbr_create();

		return self;
	}

	void
	engine_free(Engine* e)
	{
		//skybox_renderer_free(e->skybox);
		phong_free(e->phong);
		pbr_free(e->pbr);

		delete e;
	}

	void
	engine_world_draw(Engine* e, const World* w)
	{
		//enable tests and clear color and depth buffers (refactor later)
		glgpu::frame_start();

		//pack meshes to draw
		for (const auto& mesh : w->meshes)
		{
			phong_pack(e->phong, &mesh);
			//pbr_pack(e->pbr, &mesh);
		}

		//flush renderers
		{
			phong_draw(e->phong, w->cam);
			//pbr_draw(e->pbr, w->cam);
		}

		//unpack meshes
		{
			phong_unpack(e->phong);
			//pbr_unpack(e->pbr);
		}
	
		//skybox
		{
			//skybox_renderer_draw(e->skybox, w->cam);
		}
	}
};