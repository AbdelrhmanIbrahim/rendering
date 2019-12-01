#include "Engine.h"

#include "world/World.h"
#include "world/Camera.h"

#include "engine/renderers/Phong_Renderer.h"
#include "engine/renderers/PBR_Renderer.h"
#include "engine/renderers/Skybox_Renderer.h"
#include "engine/renderers/Depth_Offline_Renderer.h"

//nope (refactor later --revisit--)
#include "gpu_gl/glgpu.h"

using namespace world;

namespace rndr
{
	struct Engine
	{
		Depth_Offline_Renderer depth;
		Phong_Renderer phong;
		PBR_Renderer pbr;
		Skybox_Renderer skybox;
	};

	Engine*
	engine_create()
	{
		Engine* self = new Engine;

		self->depth = depth_offline_create();
		self->phong = phong_create();
		self->pbr = pbr_create();

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

		return self;
	}

	void
	engine_free(Engine* e)
	{
		depth_offline_free(e->depth);
		phong_free(e->phong);
		pbr_free(e->pbr);
		//skybox_renderer_free(e->skybox);

		delete e;
	}

	void
	engine_world_draw(Engine* e, const World* w)
	{
		//pack meshes to draw
		for (const auto& mesh : w->meshes)
		{
			depth_offline_pack(e->depth, &mesh);
			//phong_shadow_pack(e->phong, &mesh);
		}

		//get shadow map using depth renderer to calc shadows
		{
			//get shadow map (flush depth offline renderer from light prespective)

			//draw it using quad renderer or print it using stlb to make sure shadow map is fine
		}


		//render scene normally
		{
			//enable tests and clear color and depth buffers (refactor later)
			glgpu::frame_start();

			//flush renderers
			{
				//phong_shadow_draw(e->phong_shadow, w->cam, shadow_map);
				//phong_draw(e->phong, w->cam);
				//pbr_draw(e->pbr, w->cam);
			}

			//unpack meshes
			{
				//phong_shadow_unpack(e->phong_shadow);
				//phong_unpack(e->phong);
				//pbr_unpack(e->pbr);
			}
			//skybox
			{
				//skybox_renderer_draw(e->skybox, w->cam);
			}
		}
	}
};