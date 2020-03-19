//#include "app/App.h"
//using namespace app;

#include <Qapplication>
#include "gui/mainwindow.h"

int
main(int argc, char** argv)
{
	/*App app = app_new();
	app_run(app);
	app_free(app);*/

	QApplication app(argc, argv);
	MainWindow win;
	win.show();
	return app.exec();
}