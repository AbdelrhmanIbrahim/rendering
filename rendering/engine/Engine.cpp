#include "Engine.h"

#include "world/World.h"

#include "engine/Mesh_Renderer.h"

#include "math/Vector.h"

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
	engine_world_draw(const Engine* e, const world::World* w)
	{
		//render all meshes in the world using engine mesh renderer
		mesh_renderer_draw(e->mr, w->mesh, math::vec4f{1.0f, 0.0f, 0.0f, 1.0f});
	}
};