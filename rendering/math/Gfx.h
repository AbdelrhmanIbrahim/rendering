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

	inline Mat4f
	mat4_from_transform(const Transform& transform)
	{
		return mat4_transform(transform.rot_axis, transform.rot_angle, transform.scale, transform.translation);
	}
};