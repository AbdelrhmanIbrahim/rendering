#include "Engine.h"

#include "world/World.h"
#include "world/Camera.h"

#include "engine/renderers/Phong_Renderer.h"
#include "engine/renderers/PBR_Renderer.h"

using namespace world;

namespace rndr
{
	struct Engine
	{
		Phong_Renderer phong;
		PBR_Renderer pbr;
	};

	Engine*
	engine_create()
	{
		Engine* self = new Engine;
		
		self->phong = phong_renderer_create();
		self->pbr = pbr_renderer_create();

		return self;
	}

	void
	engine_free(Engine* e)
	{
		phong_renderer_free(e->phong);
		pbr_renderer_free(e->pbr);

		delete e;
	}

	void
	engine_world_draw(const Engine* e, World* w, const math::vec2f& viewport)
	{
		//configure the camera
		camera_viewport(w->cam, viewport);

		//render all meshes in the world using engine mesh renderer (add a buffer of meshes to each renderer -- revisit)
		//phong_renderer_draw(e->phong, w->obj, w->cam);
		pbr_renderer_draw(e->pbr, w->obj, w->cam);
	}
};