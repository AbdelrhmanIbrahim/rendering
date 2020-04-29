#pragma once

#include "ecs/World.h"

#include "engine/renderer/Skybox.h"

namespace world
{
	namespace system
	{
		void
		skybox_run(rndr::Skybox sys, ecs::World& w);
	};
};