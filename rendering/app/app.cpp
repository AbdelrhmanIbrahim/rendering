#include "app/app.h"

#include "window/window.h"
#include "gpu_gl/glgpu.h"

#include "world/World.h"
#include "engine/Engine.h"

using namespace world;
using namespace rndr;
using namespace io;

namespace app
{
	constexpr static math::vec2f window_size{WIN_WIDTH, WIN_HEIGHT};
	application::application(int argc, char** argv)
	{
		backend::callbacks_init(argc, argv);
		win::window_create((unsigned int)window_size[0], (unsigned int)window_size[1], "rendering journey");
		backend::callbacks_set(this);
		glgpu::graphics_init();

		i = Input{};
		i.mouse_x = (unsigned int)window_size[0] / 2;
		i.mouse_y = (unsigned int)window_size[1] / 2;

		w = world_create();
		e = engine_create();
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
		//frame delta
		int c_frame = backend::ticks();
		int frame_delta = c_frame - i.p_frame;
		i.p_frame = c_frame;
		camera_move(w->cam, i.keys, 0.01f * frame_delta);

		//render the top world
		engine_world_draw(e, w, window_size);
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
};