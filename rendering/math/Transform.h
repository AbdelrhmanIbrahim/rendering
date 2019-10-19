#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"

namespace math
{
	struct Transform
	{
		vec3f rot_axis;
		float rot_angle;
		vec3f scale;
		vec3f translation;
	};
};