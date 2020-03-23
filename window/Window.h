#pragma once

#include "math/Vector.h"

#include "io/Event.h"

namespace win
{
	typedef struct IWindow* Window;

	Window
	window_test_new(void* handle, unsigned int width, unsigned int height, const char* title);

	Window
	window_new(unsigned int width, unsigned int height, const char* title);

	Window
	window_fake_new(unsigned int width, unsigned int height, const char* title);

	void
	window_free(Window win);

	void
	window_swap(Window win);

	void*
	window_dc(Window win);

	void*
	window_handle(Window win);

	io::Event
	window_poll(Window win);

	math::vec2f
	window_size(Window win);

	void
	window_pixel_format_set(win::Window win);
};