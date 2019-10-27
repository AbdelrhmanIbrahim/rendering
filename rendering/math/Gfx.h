#pragma once

#include "math/Transform.h"
#include "math/Vector.h"

namespace math
{
	constexpr float PI = 3.14159265358979;

	inline Mat4f
	mat4_from_transform(const Transform& transform)
	{
		return mat4_transform(transform.rot_axis, transform.rot_angle, transform.scale, transform.translation);
	}
	
	inline float
	dot(const math::vec3f& vec)
	{
		return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
	}

	inline float
	len(const vec3f& vec)
	{
		return sqrtf(dot(vec));
	}

	inline float
	len_sqrd(const math::vec3f& vec)
	{
		return dot(vec);
	}

	inline math::vec3f
	normalize(const math::vec3f& vec)
	{
		return vec * (1 / len(vec));
	}

	inline vec3f
	cross(const math::vec3f& first, const math::vec3f& second)
	{
		return vec3f{first[1]*second[2] - second[1]*first[2] , -(first[0]*second[2]-second[0]*first[2]), first[0]*second[1]-second[0]*first[1]};
	}
};