#pragma once

#include "ecs/World.h"
#include "engine/renderer/PBR.h"

namespace world
{
	namespace system
	{
		void
		pbr_render(rndr::PBR phong, ecs::World& w);
	};
};