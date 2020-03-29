#pragma once

#include "ecs/World.h"

namespace world
{
	ecs::World
	world_create();

	void
	world_free(ecs::World& w);
};