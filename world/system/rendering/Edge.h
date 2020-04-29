#pragma once

#include "ecs/World.h"

#include "engine/renderer/Edge.h"

namespace world
{
	namespace system
	{
		void
		edge_run(rndr::Edge edge, ecs::World& w);
	};
};