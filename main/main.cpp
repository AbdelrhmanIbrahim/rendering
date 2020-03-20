//#include <Qapplication>
//#include "gui/mainwindow.h"

#include "app/App.h"
using namespace app;

int
main(int argc, char** argv)
{
	App app = app_new();
	app_run(app);
	app_free(app);
	return 0;

	//QApplication app(argc, argv);
	//gui::MainWindow win;
	//win.show();
	//return app.exec();
}