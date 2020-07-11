#pragma once

#include "defs/Defs.h"

#include "math/Matrix.h"

#include <corecrt_math.h>

namespace math
{
	struct Euler_Angles
	{
		unsigned int x_pitch_degree;
		unsigned int y_head_degree;
		unsigned int z_roll_degree;
	};

	static inline math::Mat4f
	euler_angles_transform(const Euler_Angles& angles)
	{
		float p = to_radian(angles.x_pitch_degree % 360);
		float h = to_radian(angles.y_head_degree  % 360);
		float r = to_radian(angles.z_roll_degree  % 360);

		//concat z-x-y <- right to left, col
		return Mat4f
		{
			cos(r) * cos(h) - sin(r) * sin(p) * sin(h),
			-sin(r) * cos(p),
			cos(r) * sin(h) + sin(r) * sin(p) * cos(h),
			0,

			sin(r) * cos(h) + cos(r) * sin(p) * sin(h),
			cos(r) * cos(p),
			sin(r) * sin(h) - cos(r) * sin(p) * cos(h),
			0,

			-cos(p) * sin(h),
			sin(p),
			cos(p) * cos(h),
			0,

			0,
			0,
			0,
			1 
		};
	}
};