#pragma once

#include "window/Window.h"

namespace app
{
	typedef struct IPainter* Painter;

	Painter
	painter_new();

	void
	painter_input(Painter app, win::Window_Event event);

	void
	painter_update(Painter app, int window_width, int window_height);

	void
	painter_paint(Painter app, win::Window palette);

	bool
	painter_drawing(Painter app);

	void
	painter_free(Painter app);
};