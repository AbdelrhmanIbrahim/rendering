#include "Engine.h"

#include "world/World.h"
#include "world/Camera.h"

#include "engine/Mesh_Renderer.h"

using namespace world;

namespace rndr
{
	struct Engine
	{
		Mesh_Renderer mr;
	};

	Engine*
	engine_create()
	{
		Engine* self = new Engine;
		self->mr = mesh_renderer_create();
		return self;
	}

	void
	engine_free(Engine* e)
	{
		mesh_renderer_free(e->mr);
		delete e;
	}

	void
	engine_world_draw(const Engine* e, World* w, const math::vec2f& viewport)
	{
		//configure the camera
		camera_view_config(w->cam, math::vec3f{ 0, 1, 10 }, math::vec3f{ 0, 0, 0 }, math::vec3f{ 0, 0, 1});
		camera_viewport(w->cam, viewport);

		//render all meshes in the world using engine mesh renderer
		mesh_renderer_draw(e->mr, w->obj, w->cam);
	}
};