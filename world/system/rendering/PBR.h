#pragma once

#include "ecs/World.h"

#include "engine/renderer/PBR.h"

namespace world
{
	namespace system
	{
		void
		pbr_sys_run(rndr::PBR pbr, ecs::World& w);
	};
};