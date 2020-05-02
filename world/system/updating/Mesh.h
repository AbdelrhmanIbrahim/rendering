#pragma once

#include "ecs/World.h"
#include "ecs/Entity.h"

#include "io/Input.h"

namespace world
{
	namespace system
	{
		void
		mesh_input_entity_run(ecs::World& w, io::Input& i, ecs::Entity e);

		void
		mesh_input_all_run(ecs::World& w, io::Input& i);
	}
}