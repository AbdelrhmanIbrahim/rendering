#include "world/system/updating/Camera.h"
#include "world/component/Camera.h"

namespace world
{
	namespace system
	{
		//internals
		void
		_cam_update(world::Camera* cam, const io::Input& i)
		{
			//Mouse move
			math::vec2f mouse_dir{ io::input_mouse_delta(i) };
			if (mouse_dir != math::vec2f{})
				camera_rotate(*cam, mouse_dir);

			//Keyboard
			constexpr float speed = 0.05f * 2.0f;
			if (i.keyboard[(int)io::KEYBOARD::W] == true)
				camera_move_forward(*cam, speed);
			if (i.keyboard[(int)io::KEYBOARD::S] == true)
				camera_move_backward(*cam, speed);
			if (i.keyboard[(int)io::KEYBOARD::A] == true)
				camera_move_left(*cam, speed);
			if (i.keyboard[(int)io::KEYBOARD::D] == true)
				camera_move_right(*cam, speed);

			//Wheel
			camera_zoom(*cam, i.wheel_dir);

		}

		//API
		void
		camera_viewport_all_run(ecs::World& w, math::vec2f win_size)
		{
			//fetch system req components
			auto cbag = ecs::world_active_components<world::Camera>(w);
			for (int x = 0; x < cbag.size; ++x)
				camera_viewport(cbag[x], win_size);
		}

		void
		camera_input_all_run(ecs::World& w, const io::Input& i)
		{
			//fetch system req components
			auto cbag = ecs::world_active_components<world::Camera>(w);
			for (int x = 0; x < cbag.size; ++x)
			{
				math::vec2f mouse_dir{ io::input_mouse_delta(i) };
				_cam_update(&cbag[x], i);
			}
		}

		void
		camera_input_entity_run(ecs::World& w, io::Input& i, ecs::Entity e)
		{
			auto h = ecs::world_component_add<world::Camera>(w, e);
			auto c = ecs::world_handle_component<world::Camera>(w, h);
			_cam_update(c, i);
		}
	};
};
