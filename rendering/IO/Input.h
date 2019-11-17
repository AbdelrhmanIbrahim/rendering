#pragma once

namespace io
{
	//due to glut callback not poll you cant use an Input state well to know when a stream of events finished or not (revisit)
	struct Input
	{
		//keys/mouse
		bool keys[256];

		//cursor
		int mouse_x, mouse_y;

		//current frame ticks
		int p_frame;
	};
}