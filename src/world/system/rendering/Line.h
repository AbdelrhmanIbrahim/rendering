#pragma once

#include "ecs/World.h"

#include "engine/renderer/Line.h"

namespace world
{
	namespace system
	{
		void
		line_sys_run(rndr::Line sys, ecs::World& w);
	};
};