#pragma once

namespace app
{
	typedef struct IApp* App;

	App
	app_new();

	void
	app_run(App app);

	void
	app_free(App app);
};