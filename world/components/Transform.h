#pragma once

#include "math/Vector.h"

namespace world
{
	struct Transform
	{
		float rot_angle;
		math::vec3f rot_axis;
		math::vec3f scale;
		math::vec3f translation;
	};
};