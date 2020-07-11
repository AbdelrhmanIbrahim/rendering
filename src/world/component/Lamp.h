#pragma once

#include <math/Vector.h>

namespace world
{
	struct Lamp
	{
		math::Vec4f col;
		math::Vec4f pos;
		float max;

		//std140 alignment, this should not be here -revisit-
		float dummy[3];
	};
};