#pragma once

#include "window/Window.h"

#include "math/Vector.h"

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
		int pmouse_x, pmouse_y;

		//wheel
		int wheel_dir;
	};

	void
	input_process_event(Input& input, win::Window_Event event);

	math::vec2f
	input_mouse_delta(const Input& i);

	void
	input_mouse_update(Input& i);
}