#include "world/Universe.h"

#include "world/component/Transform.h"
#include "world/component/Mesh.h"
#include "world/component/Camera.h"
#include "world/component/Material.h"
#include "world/component/Sun.h"
#include "world/component/Lamp.h"
#include "world/component/Flash.h"

#include "world/system/updating/Camera.h"

#include "fmt/printf.h"

using namespace ecs;

namespace world
{
	Universe
	universe_new()
	{
		Universe u{};
		u.world = ecs::world_new();
		u.pick_sys = world::system::pick_sys_new();

		return u;
	}

	void
	universe_free(Universe& u)
	{
		ecs::world_free(u.world);
		world::system::pick_sys_free(u.pick_sys);
	}

	bool
	universe_load_stl(Universe& u, const char* path)
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
			*data = world::Transform{ 0.0, math::Y_AXIS, math::vec3f{ 0.5, 0.5, 0.5 }, math::vec3f{0, 0, 0} };
		}

		//add material component
		{
			auto handle = world_component_add<world::Material>(u.world, stl);
			auto data = world_handle_component<world::Material>(u.world, handle);
			*data = world::Material{ math::vec4f{ 0, 1, 0, 1  }, 0.9, 0.2 };
		}

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

	void
	universe_input_act(Universe& u, math::vec2f win_size, io::Input& i, rndr::Engine engine)
	{
		//camera sys first to update viewport
		world::system::camera_sys_run(u.world, i, win_size);
		int selected_entity = world::system::pick_system_run(u.pick_sys, u.world, i, rndr::engine_colored_renderer(engine));
		if (selected_entity != -1)
			fmt::print("selected obj {} \n", selected_entity);
	}
	
	void
	universe_scripts_run(Universe& u)
	{
		world::system::script_sys_run(u.script_sys, u.world);
	}
}