#include "world/system/updating/Mesh.h"

#include "world/component/Transform.h"

namespace world
{
	namespace system
	{
		//Internals
		void
		_transform_update(world::Transform* transform, const io::Input& i)
		{
			if (i.keyboard[(int)io::KEYBOARD::W] == true)
				transform->translation[2] -= 1;
			if (i.keyboard[(int)io::KEYBOARD::S] == true)
				transform->translation[2] += 1;

			if (i.keyboard[(int)io::KEYBOARD::D] == true)
				transform->translation[0] += 1;
			if (i.keyboard[(int)io::KEYBOARD::A] == true)
				transform->translation[0] -= 1;
		}

		//API
		void
		mesh_input_entity_run(ecs::World& w, io::Input& i, ecs::Entity e)
		{
			auto h = ecs::world_component_add<world::Transform>(w, e);
			auto d = ecs::world_handle_component<world::Transform>(w, h);
			_transform_update(d, i);
		}

		void
		mesh_input_all_run(ecs::World& w, io::Input& i)
		{
			//fetch system req components
			auto tbag = ecs::world_active_components<world::Transform>(w);
			for (int x = 0; x < tbag.size; ++x)
			{
				math::vec2f mouse_dir{ io::input_mouse_delta(i) };
				_transform_update(&tbag[x], i);
			}
		}
	};
};
