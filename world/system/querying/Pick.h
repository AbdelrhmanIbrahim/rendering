#pragma once

#include "ecs/World.h"

#include "engine/renderer/Colored.h"

#include "io/Input.h"

#include "defs/Defs.h"

namespace world
{
	namespace system
	{
		typedef struct IPick_System* Pick_System;

		struct
		Pick_Info
		{
			PICKING status;
			int id;
		};

		Pick_System
		pick_sys_new();

		Pick_Info
		pick_system_run(Pick_System sys, ecs::World& w, io::Input& i, rndr::Colored colored);

		void
		pick_sys_free(Pick_System sys);
	}
}