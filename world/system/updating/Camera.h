#pragma once

#include "ecs/World.h"
#include "ecs/Entity.h"

#include "io/Input.h"

#include "math/Vector.h"

namespace world
{
	namespace system
	{
		void
		camera_input_entity_run(ecs::World& w, io::Input& i, math::vec2f win_size, ecs::Entity e);

		void
		camera_input_all_run(ecs::World& w, io::Input& i, math::vec2f win_size);
	}
}