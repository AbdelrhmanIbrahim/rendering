#pragma once

#include "ecs/World.h"
#include "engine/renderer/Colored.h"

namespace world
{
	namespace system
	{
		void
		colored_render(rndr::Colored phong, ecs::World& w);
	};
};