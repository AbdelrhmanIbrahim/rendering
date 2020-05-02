#include "world/system/updating/Camera.h"
#include "world/component/Camera.h"

namespace world
{
	namespace system
	{
		//internals
		void
		_cam_update(world::Camera* cam, const io::Input& i, math::vec2f win_size)
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

			//viewport
			camera_viewport(*cam, win_size);
		}

		//API
		void
		camera_input_entity_run(ecs::World& w, io::Input& i, math::vec2f win_size, ecs::Entity e)
		{
			auto h = ecs::world_component_add<world::Camera>(w, e);
			auto c = ecs::world_handle_component<world::Camera>(w, h);
			_cam_update(c, i, win_size);
		}

		void
		camera_input_all_run(ecs::World& w, io::Input& i, math::vec2f win_size)
		{
			//fetch system req components
			auto cbag = ecs::world_active_components<world::Camera>(w);
			for (int x = 0; x < cbag.size; ++x)
			{
				math::vec2f mouse_dir{ io::input_mouse_delta(i) };
				_cam_update(&cbag[x], i, win_size);
			}
		}
	};
};
