#include "app/app.h"

#include "window/window.h"
#include "gpu_gl/glgpu.h"

#include "world/World.h"
#include "engine/Engine.h"

#include <iostream>

using namespace world;
using namespace rndr;

namespace app
{
	math::vec2f window_size{1400, 800};

	application::application(int argc, char** argv)
	{
		backend::callbacks_init(argc, argv);
		win::window_create((unsigned int)window_size[0], (unsigned int)window_size[1], "rendering journey");
		backend::callbacks_set(this);
		glgpu::graphics_init();

		w = world_create();
		e = engine_create();
		c_mousex = (unsigned int)window_size[0] / 2;
		c_mousey = (unsigned int)window_size[1] / 2;
		p_frame = 0;
		frame_delta = 0;
		first_mouse = false;
	}

	application::~application()
	{
		world_free(w);
		engine_free(e);
	}

	void
	application::run()
	{
		backend::callbacks_run();
	}

	void
	application::update()
	{
		//render the top world
		engine_world_draw(e, w, window_size);

		//swap buffer
		backend::callbacks_update();
		int c_frame = backend::ticks();
		frame_delta = c_frame - p_frame;
		p_frame = c_frame;
	}

	void
	application::mouse_handle(int x, int y)
	{
		//screen space origin == top left
		if (first_mouse)
		{
			c_mousex = x;
			c_mousey = y;
			first_mouse = false;
		}
		math::vec2f delta{(float)x-c_mousex, (float)c_mousey-y};
		camera_rotate(w->cam, delta);
		c_mousex = x;
		c_mousey = y;
	}

	void
	application::mouse_wheel_handle(int a, int dir, int x, int y)
	{
		camera_scroll(w->cam, dir);
	}

	void
	application::keyboard_handle(unsigned char c, int x, int y)
	{
		camera_move(w->cam, c, 0.03f * frame_delta);
	}
};