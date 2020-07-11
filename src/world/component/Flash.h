#pragma once

#include <math/Vector.h>

namespace world
{
	struct Flash
	{
		math::vec4f col;
		math::vec4f pos;
		math::vec4f dir;
		float cutoff_dis;
		float penumbera_cos; //cos inner cutoff angle
		float umbera_cos; //cos outer cutoff angle

		//std140 alignment, this should not be here -revisit-
		float dummy;
	};
};