#pragma once

#include "ecs/World.h"

#include "engine/renderer/Colored.h"

#include "io/Input.h"

namespace world
{
	namespace system
	{
		typedef struct IPick_System* Pick_System;

		Pick_System
		pick_sys_new();

		int
		pick_system_run(Pick_System sys, ecs::World& w, io::Input& i, rndr::Colored colored);

		void
		pick_sys_free(Pick_System sys);
	}
}