#include <Qapplication>
#include "gui/mainwindow.h"

#include "utils/Defer.h"

#include "app/App.h"
using namespace app;

#include "gui/NativeWindow.h"

int
main(int argc, char** argv)
{
	App app = app_new();
	win::Window win = win::window_new(500, 500, "rendering");
	while (app_running(app))
	{
		//input
		win::Window_Event event = win::window_poll(win);
		app_input(app, event);

		//update
		math::vec2f window_size = win::window_size(win);
		app_update(app, window_size[0], window_size[1]);

		//render
		app_paint(app, win);
	}
	app_free(app);
	return 0;

	//QApplication qt_app(argc, argv);
	//gui::MainWindow mainWindow;
	//mainWindow.show();
	//return qt_app.exec();
}