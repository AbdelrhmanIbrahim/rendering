#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"

namespace world
{
	struct Transform
	{
		float rot_angle;
		math::vec3f rot_axis;
		math::vec3f scale;
		math::vec3f translation;
	};

	inline math::Mat4f
	mat4_from_transform(const Transform& transform)
	{
		return math::mat4_transform(transform.rot_axis, transform.rot_angle, transform.scale, transform.translation);
	}
};