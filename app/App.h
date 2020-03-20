#pragma once

#include "window/Window.h"

namespace app
{
	typedef struct IApp* App;

	App
	app_new();

	void
	app_input(App app, win::Window_Event event);

	void
	app_update(App app);

	void
	app_paint(App app, win::Window palette);

	bool
	app_running(App app);

	win::Window
	app_window(App app);

	void
	app_free(App app);
};