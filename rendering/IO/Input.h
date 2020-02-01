#pragma once

#include "window/window.h"

namespace io
{
	struct Input
	{
		//keyboard
		bool keyboard[(int)win::KEYBOARD::COUNT];

		//mouse
		bool mouse[(int)win::MOUSE::COUNT];

		//cursor
		int mouse_x, mouse_y;

		//current frame ticks
		int p_frame;
	};

	void
	input_process_event(Input input, win::Window_Event event);
}