#pragma once

namespace world
{
	struct World;
}

namespace rndr
{
	struct Engine;

	Engine*
	engine_create();

	void
	engine_free(Engine* e);

	void
	engine_world_draw(const Engine* e, const world::World* w);
}