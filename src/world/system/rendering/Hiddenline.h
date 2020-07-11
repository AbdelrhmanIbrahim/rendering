#pragma once

#include "ecs/World.h"

#include "engine/renderer/Hiddenline.h"

namespace world
{
	namespace system
	{
		void
		hiddenline_sys_run(rndr::Hiddenline hline, ecs::World& w);
	};
};