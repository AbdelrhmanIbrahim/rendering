#pragma once

#include "ecs/World.h"

#include "engine/renderer/Colored.h"
#include "engine/renderer/Edge.h"

namespace world
{
	namespace system
	{
		void
		hiddenline_run(rndr::Colored colored, rndr::Edge edge, ecs::World& w);
	};
};