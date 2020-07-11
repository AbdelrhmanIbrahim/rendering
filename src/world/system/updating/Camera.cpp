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
			//Mouse
			math::Vec2f mouse_dir{ io::input_mouse_delta(i) };
			if (mouse_dir != math::Vec2f{})
			{
				if (i.mouse[(int)io::MOUSE::RIGHT] == true)
					camera_rotate(*cam, mouse_dir);
				if (i.mouse[(int)io::MOUSE::LEFT] == true)
					camera_orbit(*cam, mouse_dir);
			}

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
		camera_viewport_all_run(ecs::World& w, math::Vec2f win_size)
		{
			//mark active cam
			int active_cam_index = 0;

			//fetch system req components
			auto& cbag = ecs::world_active_components<world::Camera>(w)[active_cam_index];
			camera_viewport(cbag, win_size);
		}

		void
		camera_input_all_run(ecs::World& w, const io::Input& i)
		{
			//mark active cam
			int active_cam_index = 0;

			//fetch system req components
			auto& cbag = ecs::world_active_components<world::Camera>(w)[active_cam_index];
			_cam_update(&cbag, i);
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
