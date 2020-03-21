#include <Qapplication>
#include "gui/mainwindow.h"

#include "utils/Defer.h"

#include "app/App.h"
using namespace app;

#include "gui/NativeWindow.h"

int
main(int argc, char** argv)
{
	//App app = app_new();
	//win::Window win = app_window(app);
	//while (app_running(app))
	//{
	//	win::Window_Event event = win::window_poll(win);
	//	app_input(app, event);
	//	app_update(app, win::window_size(win));
	//	app_paint(app, win);
	//}
	//app_free(app);
	//return 0;

	QApplication qt_app(argc, argv);
	gui::MainWindow mainWindow;
	mainWindow.show();
	return qt_app.exec();
}