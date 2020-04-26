#pragma once

#include "ecs/World.h"
#include "engine/renderer/PBR.h"

namespace world
{
	namespace system
	{
		struct
		PBR_System
		{
			rndr::PBR pbr;
		};

		PBR_System
		pbr_new();

		void
		pbr_run(PBR_System sys, ecs::World& w);

		void
		pbr_free(PBR_System sys);
	};
};