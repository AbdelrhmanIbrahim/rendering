#include "app/app.h"
#include "callbacks/callbacks.h"
#include "window/window.h"

namespace app
{
	application::application(int argc, char** argv)
	{
		cbs::callbacks_init(argc, argv);
		win::window_create(1400, 800, "rendering journey");
		cbs::callbacks_set(this);
	}

	void
	application::run()
	{
		cbs::callbacks_run();
	}

	void
	application::update()
	{
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