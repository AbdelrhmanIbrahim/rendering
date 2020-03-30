#pragma once

#include "world/ECS_World.h"

//for imgui, revisit later
#include "io/Input.h"

namespace world
{
	struct World;
}

namespace rndr
{
	typedef struct IEngine* Engine;

	Engine
	engine_create();

	void
	engine_free(Engine e);

	void
	engine_world_draw(Engine e, const world::World* w, void* win);

	void
	_engine_world_draw(Engine e, ecs::World& w, void* win);

	void
	engine_imgui_draw(Engine e, const io::Input& app_i, void* win, unsigned int width, unsigned int height);
}