#include <Qapplication>
#include "gui/mainwindow.h"
#include "gui/NativeWindow.h"

#include "utils/Defer.h"

#include "app/Painter.h"
using namespace app;

int
main(int argc, char** argv)
{
	Painter Picasso = painter_new();
	win::Window win = win::window_new(500, 500, "rendering");
	defer(win_free, win::window_free(win));
	while (painter_drawing(Picasso))
	{
		//input
		win::Window_Event event = win::window_poll(win);
		painter_input(Picasso, event);

		//update
		math::vec2f window_size = win::window_size(win);
		painter_update(Picasso, window_size[0], window_size[1]);

		//render
		painter_paint(Picasso, win);
	}
	painter_free(Picasso);
	return 0;

	//QApplication qt_app(argc, argv);
	//gui::MainWindow mainWindow;
	//mainWindow.show();
	//return qt_app.exec();
}