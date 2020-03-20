//#include <Qapplication>
//#include "gui/mainwindow.h"

#include "app/App.h"
using namespace app;

int
main(int argc, char** argv)
{
	App app = app_new();
	win::Window win = app_window(app);
	while (app_running(app))
	{
		win::Window_Event event = win::window_poll(win);
		app_input(app, event);
		app_update(app);
		app_paint(app, win);
	}
	app_free(app);
	return 0;

	//QApplication app(argc, argv);
	//gui::MainWindow win;
	//win.show();
	//return app.exec();
}