#include "ECS_World.h"

#include "components/Transform.h"
#include "components/Mesh.h"
#include "components/Camera.h"

#include "math/Vector.h"

using namespace ecs;

namespace world
{
	ecs::World
	world_create()
	{
		ecs::World w{};

		//testing
		{
			auto e1 = world_entity_new(w);
			auto e2 = world_entity_new(w);
			auto e3 = world_entity_new(w);

			//cam
			{
				auto handle_c = world_component_add<world::Camera>(w, e1);
				auto data_c = world_handle_component<world::Camera>(w, handle_c);
				*data_c = camera_new();
			}

			//sphere 1
			{
				auto handle_m = world_component_add<world::Mesh>(w, e2);
				auto data_m = world_handle_component<world::Mesh>(w, handle_m);
				*data_m = world::mesh_create(DIR_PATH"/res/stls/sphere.stl");

				auto handle_t = world_component_add<world::Transform>(w, e2);
				auto data_t = world_handle_component<world::Transform>(w, handle_t);
				*data_t = world::Transform{ 0.0, math::Y_AXIS, math::vec3f{ 1,1,1 }, math::vec3f{ -2,1,-20 } };
			}

			//sphere 2
			{
				auto handle_m = world_component_add<world::Mesh>(w, e3);
				auto data_m = world_handle_component<world::Mesh>(w, handle_m);
				*data_m = world::mesh_create(DIR_PATH"/res/stls/sphere.stl");

				auto handle_t = world_component_add<world::Transform>(w, e3);
				auto data_t = world_handle_component<world::Transform>(w, handle_t);
				*data_t = world::Transform{ 0.0, math::Y_AXIS, math::vec3f{ 1,1,1 }, math::vec3f{ 2,1,1 } };
			}
		}

		return w;
	}

	void
	world_free(ecs::World& w)
	{
		ecs::world_free(w);
	}
}