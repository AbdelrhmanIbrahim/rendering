#pragma once

#include "engine/Mesh_Renderer.h"

namespace world
{
	struct World;
}

namespace rndr
{
	struct Engine
	{
		Mesh_Renderer mr;
	};

	Engine
	engine_create();

	void
	engine_free(Engine& e);

	void
	engine_world_draw(const Engine& e, const world::World* w);
}