#include "Engine.h"

#include "world/World.h"
#include "world/Camera.h"

#include "engine/renderers/PBR_Renderer.h"

using namespace world;

namespace rndr
{
	struct Engine
	{
		PBR_Renderer mr;
	};

	Engine*
	engine_create()
	{
		Engine* self = new Engine;
		self->mr = pbr_renderer_create();
		return self;
	}

	void
	engine_free(Engine* e)
	{
		pbr_renderer_free(e->mr);
		delete e;
	}

	void
	engine_world_draw(const Engine* e, World* w, const math::vec2f& viewport)
	{
		//configure the camera
		camera_viewport(w->cam, viewport);

		//render all meshes in the world using engine mesh renderer (add a buffer of meshes to each renderer -- revisit)
		pbr_renderer_draw(e->mr, w->obj, w->cam);
	}
};