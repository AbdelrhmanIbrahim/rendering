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
	app_paint(App app, win::Window palette);

	void
	app_run(App app);

	void
	app_free(App app);
};