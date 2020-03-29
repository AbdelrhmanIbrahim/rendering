#pragma once

#include "io/Event.h"

namespace win
{
	typedef struct IWindow* Window;

	Window
	window_new(unsigned int width, unsigned int height, const char* title);

	Window
	window_fake_new(unsigned int width, unsigned int height, const char* title);

	void
	window_free(Window win);

	void
	window_swap(void* win);

	void*
	window_dc(Window win);

	io::Event
	window_poll(Window win);

	void
	window_pixel_format_set(void* win);
};