#pragma once

#include "ecs/World.h"

#include "engine/renderer/Colored.h"

namespace world
{
	namespace system
	{
		void
		colored_sys_run(rndr::Colored colored, ecs::World& w);
	};
};