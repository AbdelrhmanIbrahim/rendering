#pragma once

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
	engine_world_draw(Engine e, const world::World* w);
}