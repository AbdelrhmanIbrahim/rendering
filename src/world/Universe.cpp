#include "world/Universe.h"

#include "world/component/Transform.h"
#include "world/component/Mesh.h"
#include "world/component/Camera.h"
#include "world/component/Material.h"
#include "world/component/Sun.h"
#include "world/component/Lamp.h"
#include "world/component/Flash.h"

#include "world/system/updating/Camera.h"
#include "world/system/updating/Mesh.h"

using namespace ecs;

namespace world
{
	//internals
	void
	_universe_init_scene(Universe& u)
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
			*data_t = world::Transform{ 0.0, math::Y_AXIS, math::Vec3f{ 1,1,1 }, math::Vec3f{ 0, 0, -6} };

			auto handle_c = world_component_add<world::Material>(w, e);
			auto data_c = world_handle_component<world::Material>(w, handle_t);
			*data_c = world::Material{ math::Vec4f{ 0.75, 0.75, 0.75, 1.0f  }, 0.9, 0.2 };
		}

		//sphere 2
		{
			auto e = world_entity_new(w);
			auto handle_m = world_component_add<world::Mesh>(w, e);
			auto data_m = world_handle_component<world::Mesh>(w, handle_m);
			*data_m = world::mesh_create(DIR_PATH"/res/stls/sphere.stl");

			auto handle_t = world_component_add<world::Transform>(w, e);
			auto data_t = world_handle_component<world::Transform>(w, handle_t);
			*data_t = world::Transform{ 0.0, math::Y_AXIS, math::Vec3f{ 1,1,1 }, math::Vec3f{ 1, 0, 2 } };

			auto handle_c = world_component_add<world::Material>(w, e);
			auto data_c = world_handle_component<world::Material>(w, handle_t);
			*data_c = world::Material{ math::Vec4f{  0.75, 0.75, 0.75, 1.0f  }, 0.9, 0.2 };
		}

		//lights
		{
			
			//directional
			{
				auto e = world_entity_new(w);
				auto handle_f = world_component_add<world::Sun>(w, e);
				auto data_f = world_handle_component<world::Sun>(w, handle_f);
				*data_f = world::Sun{ {1.0f, 1.0f, 1.0f, 1.0f}, {0, -1, 0, 0} };
			}

			//point light
			{
				auto e = world_entity_new(w);
				auto handle_f = world_component_add<world::Lamp>(w, e);
				auto data_f = world_handle_component<world::Lamp>(w, handle_f);
				*data_f = world::Lamp{ {1.0f, 1.0f, 0.0f, 1.0f}, {0, 2, 0, 0}, 8};
			}
		}
	}

	//API
	Universe
	universe_new()
	{
		Universe u{};
		u.world = ecs::world_new();
		_universe_init_scene(u);

		return u;
	}

	void
	universe_free(Universe& u)
	{
		ecs::world_free(u.world);
	}

	bool
	universe_3dobject_add(Universe& u, const char* path)
	{
		//create entity
		auto stl = world_entity_new(u.world);

		//add mesh comp
		{
			auto handle = world_component_add<world::Mesh>(u.world, stl);
			auto data = world_handle_component<world::Mesh>(u.world, handle);
			*data = world::mesh_create(path);
			if (data->vertices.empty())
				return false;
		}

		//add transform component
		{
			auto handle = world_component_add<world::Transform>(u.world, stl);
			auto data = world_handle_component<world::Transform>(u.world, handle);
			*data = world::Transform{ 0.0, math::Y_AXIS, math::Vec3f{ 0.5, 0.5, 0.5 }, math::Vec3f{0, 0, 0} };
		}

		//add material component
		{
			auto handle = world_component_add<world::Material>(u.world, stl);
			auto data = world_handle_component<world::Material>(u.world, handle);
			*data = world::Material{ math::Vec4f{ 0, 1, 0, 1  }, 0.9, 0.2 };
		}

		return true;
	}

	void
	universe_update_sys_run(Universe& u, math::Vec2f win_size, const io::Input& i)
	{
		world::system::camera_viewport_all_run(u.world, win_size);
		world::system::camera_input_all_run(u.world, i);
	}

	void
	universe_movement_sys_run(Universe& u, const io::Input& i, ecs::Entity e)
	{
		if (e == ecs::INVALID_ENTITY)
			return;

		world::system::mesh_input_entity_run(u.world, i, e);
	}

	void
	universe_scripts_sys_run(Universe& u)
	{
		world::system::script_sys_run(u.script_sys, u.world);
	}
}