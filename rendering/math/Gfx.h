#pragma once

#define to_radian(degree) degree * 0.01745329251
#define to_degree(radian) radian * 57.2957795131

#include "math/Vector.h"

#include <math.h>

namespace math
{
	constexpr float PI = 3.14159265358979f;

	inline float
	dot(const math::vec3f& first, const math::vec3f& second)
	{
		return first[0] * second[0] + first[1] * second[1] + first[2] * second[2];
	}

	inline float
	dot(const math::vec4f& first, const math::vec4f& second)
	{
		return first[0] * second[0] + first[1] * second[1] + first[2] * second[2] + first[3] * second[3];
	}

	inline float
	len_sqrd(const math::vec3f& vec)
	{
		return dot(vec, vec);
	}

	inline float
	len(const vec3f& vec)
	{
		return sqrtf(len_sqrd(vec));
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