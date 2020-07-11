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
		camera_viewport_all_run(ecs::World& w, math::Vec2f win_size);

		void
		camera_input_all_run(ecs::World& w, const io::Input& i);

		void
		camera_input_entity_run(ecs::World& w, io::Input& i, ecs::Entity e);
	}
}