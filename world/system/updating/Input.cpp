#include "world/system/updating/Input.h"

#include "world/component/Input_Handle.h"
#include "world/component/Camera.h"

namespace world
{
	namespace system
	{
		void
		input_sys_erun(ecs::World& w, ecs::Entity e, const io::Input& i)
		{
			auto h = ecs::world_component_add<world::Input_Handle>(w, e);
			auto d = ecs::world_handle_component<world::Input_Handle>(w, h);
			d->act(w, e, i);
		}

		//entiites input handling fns
		void
		input_camera_act(ecs::World& w, ecs::Entity e, const io::Input& i)
		{
			auto h = ecs::world_component_add<world::Camera>(w, e);
			auto cam = ecs::world_handle_component<world::Camera>(w, h);

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
	}
}