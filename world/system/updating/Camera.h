#pragma once

#include "ecs/World.h"

#include "io/Input.h"

#include "math/Vector.h"

namespace world
{
	namespace system
	{
		void
		camera_sys_run(ecs::World& w, io::Input& i, math::vec2f win_size);
	}
}