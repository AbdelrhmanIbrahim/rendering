#include "Engine.h"

#include "world/World.h"

namespace rndr
{
	Engine
	engine_create()
	{
		Engine self{};
		self.mr = mesh_renderer_create();
		return self;
	}

	void
	engine_free(Engine & e)
	{
		mesh_renderer_free(e.mr);
	}

	void
	engine_world_draw(const Engine& e, const world::World* w)
	{
		//render all meshes in the world using engine mesh renderer
		mesh_rendere_draw(e.mr, w->mesh);
	}
};