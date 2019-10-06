#include "app/app.h"
#include "window/window.h"

namespace app
{
	application::application(int argc, char** argv)
	{
		backend::callbacks_init(argc, argv);
		win::window_create(1400, 800, "rendering journey");
		backend::callbacks_set(this);
		glgpu::graphics_init();

		w = world::world_create();
		e = rndr::engine_create();
	}

	application::~application()
	{
		world::world_free(w);
		rndr::engine_free(e);
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
		rndr::engine_world_draw(e, &w);

		//swap buffer
		backend::callbacks_update();
	}

	void
	application::mouse_handle(int x, int y)
	{
	}

	void
	application::keyboard_handle(unsigned char c, int x, int y)
	{
	}
};