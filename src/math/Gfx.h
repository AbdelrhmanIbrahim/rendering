#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"

#include <math.h>

namespace math
{
	constexpr float PI = 3.14159265358979f;

	inline float
	dot(const math::Vec3f& first, const math::Vec3f& second)
	{
		return first[0] * second[0] + first[1] * second[1] + first[2] * second[2];
	}

	inline float
	dot(const math::Vec4f& first, const math::Vec4f& second)
	{
		return first[0] * second[0] + first[1] * second[1] + first[2] * second[2] + first[3] * second[3];
	}

	inline float
	len_sqrd(const math::Vec3f& vec)
	{
		return dot(vec, vec);
	}

	inline float
	len(const Vec3f& vec)
	{
		return sqrtf(len_sqrd(vec));
	}

	inline math::Vec3f
	normalize(const math::Vec3f& vec)
	{
		return vec * (1 / len(vec));
	}

	inline Vec3f
	cross(const math::Vec3f& first, const math::Vec3f& second)
	{
		return Vec3f{first[1]*second[2] - second[1]*first[2] , -(first[0]*second[2]-second[0]*first[2]), first[0]*second[1]-second[0]*first[1]};
	}

	inline math::Mat4f
	view_matrix(const math::Vec3f& fwd, const math::Vec3f& right, const math::Vec3f& up, const math::Vec3f& eye)
	{
		math::Mat4f view{};
		view[0] = math::Vec4f{ right[0], up[0], -fwd[0], 0 };
		view[1] = math::Vec4f{ right[1], up[1], -fwd[1], 0 };
		view[2] = math::Vec4f{ right[2], up[2], -fwd[2], 0 };
		view[3] = math::Vec4f{ -dot(eye, right), -dot(eye, up), dot(eye, fwd), 1 };
		return view;
	}

	inline math::Mat4f
	view_lookat_matrix(const math::Vec3f& eye, const math::Vec3f& target, const math::Vec3f& up)
	{
		Vec3f fwd = math::normalize(target - eye);
		Vec3f right = math::normalize(math::cross(fwd, up));
		Vec3f r_up = math::normalize(math::cross(right, fwd));
		return view_matrix(fwd, right, r_up, eye);
	}

	inline math::Mat4f
	proj_prespective_matrix(float f, float n, float r, float l, float t, float b, float fov_tan)
	{
		math::Mat4f proj{};
		float distance = f - n;
		float aspect_ratio = (r - l) / (t - b);
		proj[0][0] = 1.0f / (aspect_ratio * fov_tan);
		proj[1][1] = 1.0f / fov_tan;
		proj[2][2] = -(n + f) / distance;
		proj[2][3] = -1.0f;
		proj[3][2] = -2.0f*f*n / distance;

		return proj;
	}

	inline math::Mat4f
	proj_ortho_matrix(float f, float n, float r, float l, float t, float b)
	{
		math::Mat4f proj{};
		proj[0][0] = 2 / (r - l);
		proj[1][1] = 2 / (t - b);
		proj[2][2] = -1 / (f - n);
		proj[3][0] = -(r + l) / (r - l);
		proj[3][1] = -(t + b) / (t - b);
		proj[3][2] = -(n) / (f - n);
		proj[3][3] = 1;

		return proj;
	}
};