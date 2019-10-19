#pragma once

#include "math/Transform.h"

namespace math
{
	inline Mat4f
	mat4_from_transform(const Transform& transform)
	{
		return mat4_transform(transform.rot_axis, transform.rot_angle, transform.scale, transform.translation);
	}
};