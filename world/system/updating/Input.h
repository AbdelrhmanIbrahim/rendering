#pragma once

#include "ecs/Entity.h"
#include "ecs/World.h"

#include "io/Input.h"

namespace world
{
	namespace system
	{
		void
		input_sys_erun(ecs::World& w, ecs::Entity e, const io::Input& i);

		template<typename C>
		void
		input_sys_run(ecs::World& w, const io::Input& i)
		{
			auto entities_bag = ecs::world_active_entities<C>(w);
			for (int x = 0; x < entities_bag.size; ++x)
				input_sys_erun(w, entities_bag[x], i);
		}

		//entities input handling fns (must be with this signature)
		void
		input_camera_act(ecs::World& w, ecs::Entity e, const io::Input& i);
	}
}