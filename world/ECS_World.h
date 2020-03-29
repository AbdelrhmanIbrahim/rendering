#pragma once

#include "ecs/World.h"

namespace world
{
	ecs::World
	_world_create();

	void
	_world_free(ecs::World& w);
};