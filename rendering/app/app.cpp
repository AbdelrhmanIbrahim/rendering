#include "app/app.h"

#include "Defs.h"

#include "gpu_gl/glgpu.h"

#include <iostream>

using namespace world;
using namespace rndr;
using namespace io;

namespace app
{
	void
	_input_act(const Input& i, World* w)
	{
		//Mouse
		{
			if (i.mouse[(int)win::MOUSE::RIGHT] == true)
				std::cout << "mouse right down";
			/*else if (i.mouse[(int)win::MOUSE::RIGHT] == false)
				std::cout << "mouse right up";*/

			if (i.mouse[(int)win::MOUSE::LEFT] == true)
				std::cout << "mouse left down";
			/*else if (i.mouse[(int)win::MOUSE::LEFT] == false)
				std::cout << "mouse left up";*/

			if (i.mouse[(int)win::MOUSE::MIDDLE] == true)
				std::cout << "mouse middle down";
			/*else if (i.mouse[(int)win::MOUSE::MIDDLE] == false)
				std::cout << "mouse middle up";*/
		}

		//Keyboard
		{
			if (i.keyboard[(int)win::KEYBOARD::W] == true)
				std::cout << "move forward";
			/*else if (i.keyboard[(int)win::KEYBOARD::W] == false)
				std::cout << "stop W";*/

			if (i.keyboard[(int)win::KEYBOARD::S] == true)
				std::cout << "move back";
			/*else if (i.keyboard[(int)win::KEYBOARD::S] == false)
				std::cout << "stop S";*/

			if (i.keyboard[(int)win::KEYBOARD::A] == true)
				std::cout << "move left";
			/*else if (i.keyboard[(int)win::KEYBOARD::A] == false)
				std::cout << "stop A";*/

			if (i.keyboard[(int)win::KEYBOARD::D] == true)
				std::cout << "move right";
			/*else if (i.keyboard[(int)win::KEYBOARD::D] == false)
				std::cout << "stop D";*/

			float delta = 2.0f; //TODO
			//camera_move(w->cam, i.keyboard, 0.05f * delta);
		}

		//Mouse move
		{
			//std::cout << "mouse move";
			//camera_rotate(w->cam, input_mouse_delta(i));
			//input_mouse_update(i);
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

		//e = engine_create();
		//w = world_create();
	}

	application::~application()
	{
		//world_free(w);
		//engine_free(e);

		win::window_free(win);
		glgpu::context_free(ctx);
	}

	void
	application::run()
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
		{
			//break the main loop
			//destruct app
		}

		//send event to input
		input_process_event(i, event);

		//call the right procedures according to the input state to update the data
		_input_act(i, w);

		//render the data
		{
			//set camera viewport to window's viewport

			//render
			glgpu::frame_start();
			glgpu::color_clear(0, 1, 0);
		}

		//swap window buffers
		window_swap(win);
	}

	void
	application::update()
	{
		//frame delta
		int c_frame = backend::ticks();
		int frame_delta = c_frame - i.p_frame;
		i.p_frame = c_frame;
		camera_move(w->cam, i.keyboard, 0.05f * frame_delta);

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