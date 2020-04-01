#pragma once

#include "io/Event.h"

#include "defs/Defs.h"

namespace app
{
	typedef struct IPainter* Painter;

	Painter
	painter_new();

	void
	painter_style(Painter app, Rendering mode);

	bool
	painter_stl_load(Painter app, const char* path);

	void
	painter_input(Painter app, io::Event event);

	void
	painter_update(Painter app, int window_width, int window_height);

	void
	painter_paint(Painter app, void* palette, unsigned int width, unsigned int height);

	void
	painter_free(Painter app);
};