#pragma once

#include "ecs/World.h"

#include "engine/renderer/Colored.h"

namespace world
{
	namespace system
	{
		struct
		Colored_System
		{
			rndr::Colored colored;
		};

		Colored_System
		colored_new();

		void
		colored_run(Colored_System sys, ecs::World& w);

		void
		colored_free(Colored_System sys);
	};
};