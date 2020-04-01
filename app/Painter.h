#pragma once

#include "io/Event.h"

namespace app
{
	typedef struct IPainter* Painter;

	Painter
	painter_new();

	void
	painter_input(Painter app, io::Event event);

	void
	painter_update(Painter app, int window_width, int window_height);

	void
	painter_paint(Painter app, void* palette, unsigned int width, unsigned int height);

	bool
	painter_drawing(Painter app);

	void
	painter_free(Painter app);
};