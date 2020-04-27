#pragma once

#include "engine/renderer/Point.h"

#include "ecs/World.h"

namespace world
{
	namespace system
	{
		struct
		Point_System
		{
			rndr::Point point;
		};

		Point_System
		point_new();

		void
		point_run(Point_System sys, ecs::World& w);

		void
		point_free(Point_System sys);
	};
};