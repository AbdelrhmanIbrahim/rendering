#pragma once

#include "math/Gfx.h"
#include "math/Vector.h"
#include "math/Matrix.h"

namespace math
{
	//unit quaternion
	struct Quat
	{
		float x, y, z, w;

		inline Quat
		operator-() const
		{
			return Quat{ -x,-y,-z, w };
		}

		inline Quat
		operator*(const Quat& other) const
		{
			Vec3f quat_img = Vec3f{ x, y, z };
			Vec3f other_img = Vec3f{ other.x, other.y, other.z };
			Vec3f res_img = cross(quat_img, other_img) + other_img*w + quat_img*other.w;
			float res_real = w*other.w - dot(quat_img, other_img);
			return Quat{ res_img[0], res_img[1], res_img[2], res_real };
		}

		inline Vec3f
		operator*(const Vec3f& other) const
		{
			Vec3f img{ x, y, z };
			return 2.0f * dot(img, other) * img + (w*w - dot(img, img)) * other + 2.0f * w * cross(img, other);
		}
	};

	inline Quat
	quat_from_axis(const math::Vec3f& axis_normal, float theta_radian)
	{
		float theta_half = theta_radian / 2;
		float w = cos(theta_half);
		float x = axis_normal[0] * sin(theta_half);
		float y = axis_normal[1] * sin(theta_half);
		float z = axis_normal[2] * sin(theta_half);
		return Quat{ x,y,z,w };
	}

	inline Mat4f
	quat_to_matrix(const Quat& quat)
	{
		float w = quat.w;
		float x = quat.x;
		float y = quat.y;
		float z = quat.z;

		return Mat4f
		{
			1 - 2 * y * y - 2 * z * z,
			2 * x * y - 2 * z * w,
			2 * x * z + 2 * y * w,
			0,

			2 * x * y + 2 * z * w,
			1 - 2 * x * x - 2 * z * z,
			2 * y * z - 2 * x * w,
			0,

			2 * x * z - 2 * y * w,
			2 * y * z + 2 * x * w,
			1 - 2 * x * x - 2 * y * y,
			0,

			0,
			0,
			0,
			1 
		};
	}
};
