#include <Qapplication>
#include "gui/MainWindow.h"

int
main(int argc, char** argv)
{

	//Painter Picasso = painter_new();
	//win::Window win = win::window_new(500, 500, "rendering");
	//defer(win_free, win::window_free(win));
	//while (painter_drawing(Picasso))
	//{
	//	//input
	//	win::Window_Event event = win::window_poll(win);
	//	painter_input(Picasso, event);

	//	//update
	//	math::Vec2f window_size = win::window_size(win);
	//	painter_update(Picasso, window_size[0], window_size[1]);

	//	//render
	//	painter_paint(Picasso, win);
	//}
	//painter_free(Picasso);
	//return 0; //test editing

	QApplication qt_app(argc, argv);
	gui::MainWindow m;
	m.show();
	return qt_app.exec();
}