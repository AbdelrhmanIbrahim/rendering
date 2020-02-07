#include "app/app.h"

#include "Defs.h"

//#include <iostream>

using namespace world;
using namespace rndr;
using namespace io;

namespace app
{
	void
	_input_act(const Input& i, World* w)
	{
		//Mouse move
		{
			camera_rotate(w->cam, input_mouse_delta(i));
		}

		//Keyboard
		{
			float delta = 2.0f; //TODO
			if (i.keyboard[(int)win::KEYBOARD::W] == true)
				camera_move_forward(w->cam, 0.05f * delta);

			if (i.keyboard[(int)win::KEYBOARD::S] == true)
				camera_move_backward(w->cam, 0.05f * delta);

			if (i.keyboard[(int)win::KEYBOARD::A] == true)
				camera_move_left(w->cam, 0.05f * delta);

			if (i.keyboard[(int)win::KEYBOARD::D] == true)
				camera_move_right(w->cam, 0.05f * delta);
		}

		//Wheel
		{
			//std::cout << i.wheel_dir << std::endl;
			camera_zoom(w->cam, i.wheel_dir);
		}
	}

	application::application(int argc, char** argv)
	{
		//window and its context
		window_size = math::vec2f{ WIN_WIDTH, WIN_HEIGHT };
		win = win::window_new(WIN_WIDTH, WIN_HEIGHT, "rendering journey");
		ctx = glgpu::context_attach(4, 0, win);

		//input init state
		i = Input{};
		i.mouse_x = WIN_WIDTH / 2;
		i.mouse_y = WIN_HEIGHT / 2;
		i.pmouse_x = WIN_WIDTH / 2;
		i.pmouse_y = WIN_HEIGHT / 2;

		e = engine_create();
		w = world_create();
	}

	application::~application()
	{
		world_free(w);
		engine_free(e);

		win::window_free(win);
		glgpu::context_free(ctx);
	}

	void
	application::run()
	{
		while (true)
		{
			//get the window input event
			auto event = window_poll(win);

			//resize or close window
			if (event.kind == win::Window_Event::KIND::KIND_WINDOW_RESIZE)
			{
				window_size[0] = event.window_resize.width;
				window_size[1] = event.window_resize.height;
			}
			else if (event.kind == win::Window_Event::KIND::KIND_WINDOW_CLOSE)
				break;

			//send event to input
			input_process_event(i, event);

			//call the right procedures according to the input state to update the data
			_input_act(i, w);
			input_mouse_update(i);

			//render the data
			{
				//set camera viewport to window's viewport
				camera_viewport(w->cam, window_size);

				//render
				engine_world_draw(e, w);
			}

			//swap window buffers
			window_swap(win);
		}
	}

	void
	application::update()
	{
		//frame delta
		int c_frame = backend::ticks();
		int frame_delta = c_frame - i.p_frame;
		i.p_frame = c_frame;
		//camera_move(w->cam, i.keyboard, 0.05f * frame_delta);

		//render the top world
		camera_viewport(w->cam, window_size);
		engine_world_draw(e, w);
		backend::callbacks_update();
	}

	void
	application::mouse_wheel_handle(int a, int dir, int x, int y)
	{
		camera_zoom(w->cam, dir);
	}
};