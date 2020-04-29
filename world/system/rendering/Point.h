#pragma once

#include "ecs/World.h"

#include "engine/renderer/Point.h"

namespace world
{
	namespace system
	{
		void
		point_run(rndr::Point point, ecs::World& w);
	};
};