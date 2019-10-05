#include "app/app.h"
#include "backend/backend.h"
#include "window/window.h"

namespace app
{
	application::application(int argc, char** argv)
	{
		backend::callbacks_init(argc, argv);
		win::window_create(1400, 800, "rendering journey");
		backend::callbacks_set(this);
		backend::graphics_init();
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