#pragma once

#include "window/Window.h"

#include "math/Vector.h"

#include "io/Event.h"

namespace io
{
	struct Input
	{
		//keyboard
		bool keyboard[(int)io::KEYBOARD::COUNT];

		//mouse
		bool mouse[(int)io::MOUSE::COUNT];

		//cursor
		int mouse_x, mouse_y;
		int pmouse_x, pmouse_y;

		//wheel
		int wheel_dir;
	};

	Input
	input_new();

	void
	input_process_event(Input& input, io::Event event);

	math::Vec2f
	input_mouse_delta(const Input& i);

	void
	input_update(Input& i);
}