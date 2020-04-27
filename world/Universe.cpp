#include "world/Universe.h"

#include "world/component/Transform.h"
#include "world/component/Mesh.h"
#include "world/component/Camera.h"
#include "world/component/Material.h"
#include "world/component/Flash.h"

using namespace ecs;

namespace world
{
	Universe
	universe_new()
	{
		Universe u{};
		u.world = ecs::world_new();

		return u;
	}

	void
	universe_free(Universe& u)
	{
		ecs::world_free(u.world);
	}

	bool
	universe_load_stl(Universe& u, const char* path)
	{
		//create entity
		auto stl = world_entity_new(u.world);

		//add mesh comp
		auto handle_m = world_component_add<world::Mesh>(u.world, stl);
		auto data_m = world_handle_component<world::Mesh>(u.world, handle_m);
		*data_m = world::mesh_create(path);
		if (data_m->vertices.empty())
			return false;

		//add transform component
		auto handle_t = world_component_add<world::Transform>(u.world, stl);
		auto data_t = world_handle_component<world::Transform>(u.world, handle_t);
		*data_t = world::Transform{ 0.0, math::Y_AXIS, math::vec3f{ 1,1,1 }, math::vec3f{0,0,-1} };

		return true;
	}

	void
	universe_init_scene(Universe& u)
	{
		ecs::World& w = u.world;

		//cam
		{
			auto e = world_entity_new(w);
			auto handle_c = world_component_add<world::Camera>(w, e);
			auto data_c = world_handle_component<world::Camera>(w, handle_c);
			*data_c = camera_new();

			auto handle_f = world_component_add<world::Flash>(w, e);
			auto data_f = world_handle_component<world::Flash>(w, handle_f);
			*data_f = world::Flash{ {1.0f, 1.0f, 1.0f, 1.0f},
									{0.0f, 0.0f, 0.0f, 0.0f },
									{0, 0, -1, 0}, 10,
									(float)cos(to_radian(12)),
									(float)cos(to_radian(15)) };

			u.script_sys.cam_flash_scripts.push_back(ecs::Script<world::Camera, world::Flash>{e, e, world::system::script_flash_cam});
		}

		//sphere 1
		{
			auto e = world_entity_new(w);
			auto handle_m = world_component_add<world::Mesh>(w, e);
			auto data_m = world_handle_component<world::Mesh>(w, handle_m);
			*data_m = world::mesh_create(DIR_PATH"/res/stls/sphere.stl");

			auto handle_t = world_component_add<world::Transform>(w, e);
			auto data_t = world_handle_component<world::Transform>(w, handle_t);
			*data_t = world::Transform{ 0.0, math::Y_AXIS, math::vec3f{ 1,1,1 }, math::vec3f{ 0, 0, -6} };

			auto handle_c = world_component_add<world::Material>(w, e);
			auto data_c = world_handle_component<world::Material>(w, handle_t);
			*data_c = world::Material{ math::vec4f{ 0.75, 0.75, 0.75, 1.0f  }, 0.9, 0.2 };
		}

		//sphere 2
		{
			auto e = world_entity_new(w);
			auto handle_m = world_component_add<world::Mesh>(w, e);
			auto data_m = world_handle_component<world::Mesh>(w, handle_m);
			*data_m = world::mesh_create(DIR_PATH"/res/stls/sphere.stl");

			auto handle_t = world_component_add<world::Transform>(w, e);
			auto data_t = world_handle_component<world::Transform>(w, handle_t);
			*data_t = world::Transform{ 0.0, math::Y_AXIS, math::vec3f{ 1,1,1 }, math::vec3f{ 1, 0, 2 } };

			auto handle_c = world_component_add<world::Material>(w, e);
			auto data_c = world_handle_component<world::Material>(w, handle_t);
			*data_c = world::Material{ math::vec4f{  0.75, 0.75, 0.75, 1.0f  }, 0.9, 0.2 };
		}
	}

	void
	universe_scripts_run(Universe& u)
	{
		world::system::script_system_run(u.script_sys, u.world);
	}
}