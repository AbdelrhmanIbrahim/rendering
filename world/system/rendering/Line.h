#pragma once

#include "engine/renderer/Line.h"

#include "ecs/World.h"

namespace world
{
	namespace system
	{
		struct
		Line_System
		{
			rndr::Line point;
		};

		Line_System
		line_new();

		void
		line_run(Line_System sys, ecs::World& w);

		void
		line_free(Line_System sys);
	};
};