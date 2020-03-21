#include <Qapplication>
#include "gui/mainwindow.h"

#include "utils/Defer.h"

#include "app/App.h"
using namespace app;

#include "Qopenglcontext"

int
main(int argc, char** argv)
{
	App app = app_new();
	win::Window win = app_window(app);
	while (app_running(app))
	{
		win::Window_Event event = win::window_poll(win);
		app_input(app, event);
		app_update(app, win::window_size(win));
		app_paint(app, win);
	}
	app_free(app);
	return 0;

	//defer(painter_free, app_free(painter));
	//QOpenGLContext* aa;
	//aa->makeCurrent()

	//QApplication qt_app(argc, argv);
	//gui::MainWindow qt_main_win;
	//qt_main_win.show();
	//return qt_app.exec();
}