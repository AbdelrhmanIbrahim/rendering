#include "app/app.h"

#include "Defs.h"

#include "gpu_gl/glgpu.h"

using namespace world;
using namespace rndr;
using namespace io;

namespace app
{
	application::application(int argc, char** argv)
	{
		//backend::callbacks_init(argc, argv);
		win = win::window_new(WIN_WIDTH, WIN_HEIGHT, "rendering journey");
		ctx = glgpu::context_attach(4, 0, win);

		//i = Input{};
		//i.mouse_x = WIN_WIDTH / 2;
		//i.mouse_y = WIN_HEIGHT / 2;

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

		//send event to input

		//call the right procedures according to the input state to update the data

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
		camera_move(w->cam, i.keys, 0.05f * frame_delta);

		//render the top world
		camera_viewport(w->cam, window_size);
		engine_world_draw(e, w);
		backend::callbacks_update();
	}

	void
	application::mouse_handle(int x, int y)
	{
		camera_rotate(w->cam, math::vec2f{ (float)x - i.mouse_x, (float)i.mouse_y - y });
		i.mouse_x = x; i.mouse_y = y;
	}

	void
	application::mouse_wheel_handle(int a, int dir, int x, int y)
	{
		camera_zoom(w->cam, dir);
	}

	void
	application::keyboard_press_handle(unsigned char c, int x, int y)
	{
		i.keys[c] = true;
	}

	void
	application::keyboard_release_handle(unsigned char c, int x, int y)
	{
		i.keys[c] = false;
	}

	void
	application::window_resize_handle(int width, int height)
	{
		window_size[0] = width;
		window_size[1] = height;
	}
};