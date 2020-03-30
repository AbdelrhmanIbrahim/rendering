#include "ECS_World.h"

#include "components/Transform.h"
#include "components/Mesh.h"
#include "components/Camera.h"

#include "math/Vector.h"

using namespace ecs;

namespace world
{
	ecs::World
	_world_create()
	{
		ecs::World w{};

		//testing
		{
			//cam
			{
				auto e = world_entity_new(w);
				auto handle_c = world_component_add<world::_Camera>(w, e);
				auto data_c = world_handle_component<world::_Camera>(w, handle_c);
				*data_c = _camera_new();
			}

			//sphere 1
			{
				auto e = world_entity_new(w);
				auto handle_m = world_component_add<world::_Mesh>(w, e);
				auto data_m = world_handle_component<world::_Mesh>(w, handle_m);
				*data_m = world::_mesh_create(DIR_PATH"/res/stls/sphere.stl");

				auto handle_t = world_component_add<world::_Transform>(w, e);
				auto data_t = world_handle_component<world::_Transform>(w, handle_t);
				*data_t = world::_Transform{ 0.0, math::Y_AXIS, math::vec3f{ 1,1,1 }, math::vec3f{ -2,1,-20 } };
			}

			//sphere 2
			{
				auto e = world_entity_new(w);
				auto handle_m = world_component_add<world::_Mesh>(w, e);
				auto data_m = world_handle_component<world::_Mesh>(w, handle_m);
				*data_m = world::_mesh_create(DIR_PATH"/res/stls/sphere.stl");

				auto handle_t = world_component_add<world::_Transform>(w, e);
				auto data_t = world_handle_component<world::_Transform>(w, handle_t);
				*data_t = world::_Transform{ 0.0, math::Y_AXIS, math::vec3f{ 1,1,1 }, math::vec3f{ 2,1,1 } };
			}
		}

		return w;
	}

	void
	_world_free(ecs::World& w)
	{
		ecs::world_free(w);
	}
}