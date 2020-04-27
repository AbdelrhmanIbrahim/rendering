#pragma once

#include "ecs/World.h"
#include "io/Input.h"

namespace world
{
	namespace system
	{
		void
		camera_system_run(ecs::World& w, io::Input& i, int win_width, int win_height);
	}
}