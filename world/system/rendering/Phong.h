#pragma once

#include "ecs/World.h"
#include "engine/renderer/Phong.h"

namespace world
{
	namespace system
	{
		void
		phong_render(rndr::Phong phong, ecs::World& w);
	};
};