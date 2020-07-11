#pragma once

#include <math/Vector.h>

namespace world
{
	struct Flash
	{
		math::Vec4f col;
		math::Vec4f pos;
		math::Vec4f dir;
		float cutoff_dis;
		float penumbera_cos; //cos inner cutoff angle
		float umbera_cos; //cos outer cutoff angle

		//std140 alignment, this should not be here -revisit-
		float dummy;
	};
};