#pragma once

#include "math/Vector.h"
#include <math.h>

namespace math
{
	/* COL Major
	X axis
	Y axis
	Z axis
	translation
	*/
	struct Mat4f
	{
		vec4f data[4];

		vec4f&
		operator[](unsigned int index)
		{
			return data[index];
		}

		const vec4f&
		operator[](unsigned int index) const
		{
			return data[index];
		}

		inline Mat4f
		operator*(const Mat4f& other)
		{
			return Mat4f 
			{
				data[0] * other[0][0] + data[1] * other[0][1] + data[2] * other[0][2] + data[3] * other[0][3],
				data[0] * other[1][0] + data[1] * other[1][1] + data[2] * other[1][2] + data[3] * other[1][3],
				data[0] * other[2][0] + data[1] * other[2][1] + data[2] * other[2][2] + data[3] * other[2][3],
				data[0] * other[3][0] + data[1] * other[3][1] + data[2] * other[3][2] + data[3] * other[3][3]
			};
		}

		inline vec4f
		operator*(const vec4f& other)
		{
			return data[0] * other[0] + data[1] * other[1] + data[2] * other[2] + data[3] * other[3];
		}
	};

	inline Mat4f
	mat4_scale(const vec4f& scale)
	{
		Mat4f m{};
		m[0][0] = scale[0];
		m[1][1] = scale[1];
		m[2][2] = scale[2];
		m[3][3] = scale[2];
		return m;
	}

	inline Mat4f
	mat4_scale(const vec3f& scale)
	{
		Mat4f m{};
		m[0][0] = scale[0];
		m[1][1] = scale[1];
		m[2][2] = scale[2];
		m[3][3] = 1;
		return m;
	}

	inline Mat4f
	mat4_id()
	{
		return mat4_scale(vec4f{ 1, 1, 1, 1 });
	}

	inline Mat4f
	mat4_translate(const vec3f& translation)
	{
		Mat4f m = mat4_id();
		m[3] = vec4f{translation[0], translation[1], translation[2], 1.0f};
		return m;
	}

	inline Mat4f
	mat4f_rotate(const vec3f& axis_normal, float rad)
	{
		Mat4f m = mat4_id();
		double cosine = cos(rad);
		double sine = sin(rad);
		float nx = axis_normal[0];
		float ny = axis_normal[1];
		float nz = axis_normal[2];

		// Rodriguries formula
		m[0][0] = nx * nx * (1 - cosine) + cosine;
		m[0][1] = ny * nx * (1 - cosine) - nz * sine;
		m[0][2] = nz * nx * (1 - cosine) + ny * sine;

		m[1][0] = nx * ny * (1 - cosine) + nz * sine;
		m[1][1] = ny * ny * (1 - cosine) + cosine;
		m[1][2] = nz * ny * (1 - cosine) - nx * sine;

		m[2][0] = nx * nz * (1 - cosine) - ny * sine;
		m[2][1] = ny * nz * (1 - cosine) + nx * sine;
		m[2][2] = nz * nz * (1 - cosine) + cosine;

		return m;
	}

	inline Mat4f
	mat4_transform(const vec3f axis, float angle_rad, const vec3f& scale, const vec3f& translation)
	{
		return mat4_translate(translation) * mat4_scale(scale) * mat4f_rotate(axis, angle_rad);
	}
};