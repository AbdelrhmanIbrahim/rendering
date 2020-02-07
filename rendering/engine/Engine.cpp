#include "Engine.h"

#include "world/World.h"
#include "world/Camera.h"

#include "engine/renderers/Phong_Renderer.h"
#include "engine/renderers/PBR_Renderer.h"
#include "engine/renderers/Skybox_Renderer.h"
#include "engine/renderers/Phong_Shadow_Renderer.h"

#include "IO/Image.h"

#include "math/Vector.h"

//nope (refactor later --revisit--)
#include "gpu_gl/glgpu.h"

using namespace world;

namespace rndr
{
	struct IEngine
	{
		Phong_Shadow_Renderer phong_shadow;
		Phong_Renderer phong;
		PBR_Renderer pbr;
		Skybox_Renderer skybox;
	};

	Engine
	engine_create()
	{
		IEngine* self = new IEngine;

		self->phong = phong_create();
		//self->phong_shadow = phong_shadow_create();
		//self->pbr = pbr_create();
		//self->skybox = skybox_renderer_hdr_create("../rendering/res/imgs/hdr/Tokyo_spec.hdr");

		//skybox
		/*static const char* skybox_paths[6]
		{
			"../rendering/res/imgs/skybox/right.jpg",
			"../rendering/res/imgs/skybox/left.jpg",
			"../rendering/res/imgs/skybox/top.jpg",
			"../rendering/res/imgs/skybox/bottom.jpg",
			"../rendering/res/imgs/skybox/front.jpg",
			"../rendering/res/imgs/skybox/back.jpg"
		};
		self->skybox = skybox_renderer_rgba_create(skybox_paths, io::IMAGE_FORMAT::JPG);*/

		return self;
	}

	void
	engine_free(Engine e)
	{
		phong_free(e->phong);
		//phong_shadow_free(e->phong_shadow);
		//pbr_free(e->pbr);
		//skybox_renderer_free(e->skybox);

		delete e;
	}

	void
	engine_world_draw(Engine e, const World* w)
	{
		//render scene normally
		{
			//enable tests and clear color and depth buffers (refactor later)
			glgpu::frame_start();

			//pack meshes to draw
			for (const auto& mesh : w->meshes)
			{
				//phong_shadow_pack(e->phong_shadow, &mesh);
				phong_pack(e->phong, &mesh);
				//pbr_pack(e->pbr, &mesh);
			}

			//flush renderers
			{
				//phong_shadow_draw(e->phong_shadow, math::vec3f{0,30,0}, w->cam);
				phong_draw(e->phong, w->cam);
				//pbr_draw(e->pbr, w->cam);
			}

			//unpack meshes
			{
				//phong_shadow_unpack(e->phong_shadow);
				phong_unpack(e->phong);
				//pbr_unpack(e->pbr);
			}

			//skybox
			{
				//skybox_renderer_draw(e->skybox, w->cam);
			}
		}
	}
};