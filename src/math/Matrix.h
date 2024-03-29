#pragma once

#include "math/Vector.h"

#include <math.h>

namespace math
{
	/* 
	Xx Yx Zx 0 
	Xy Yy Zy 0
	Xz Yz Zz 0
	Tx Ty Tz 1
	*/
	struct Mat4f
	{
		Vec4f data[4];

		Vec4f&
		operator[](unsigned int index)
		{
			return data[index];
		}

		const Vec4f&
		operator[](unsigned int index) const
		{
			return data[index];
		}

		Mat4f
		operator*(const Mat4f& other) const
		{
			return Mat4f 
			{
				data[0] * other[0][0] + data[1] * other[0][1] + data[2] * other[0][2] + data[3] * other[0][3],
				data[0] * other[1][0] + data[1] * other[1][1] + data[2] * other[1][2] + data[3] * other[1][3],
				data[0] * other[2][0] + data[1] * other[2][1] + data[2] * other[2][2] + data[3] * other[2][3],
				data[0] * other[3][0] + data[1] * other[3][1] + data[2] * other[3][2] + data[3] * other[3][3]
			};
		}

		Mat4f
		operator*(float factor) const
		{
			return Mat4f
			{
				factor * data[0],
				factor * data[1],
				factor * data[2],
				factor * data[3]
			};
		}

		Vec3f
		operator*(const Vec4f& other)
		{
			return Vec3f
			{
				data[0][0] * other[0] + data[1][0] * other[1] + data[2][0] * other[2] + data[3][0] * other[3],
				data[0][1] * other[0] + data[1][1] * other[1] + data[2][1] * other[2] + data[3][1] * other[3],
				data[0][2] * other[0] + data[1][2] * other[1] + data[2][2] * other[2] + data[3][2] * other[3]
			};
		}
	};

	//left side
	inline Mat4f
	operator*(float factor, const Mat4f& mat)
	{
		return mat * factor;
	}

	inline static Mat4f
	mat4_scale(const Vec4f& scale)
	{
		Mat4f m{};
		m[0][0] = scale[0];
		m[1][1] = scale[1];
		m[2][2] = scale[2];
		m[3][3] = scale[2];
		return m;
	}

	inline Mat4f
	mat4_scale(const Vec3f& scale)
	{
		Mat4f m{};
		m[0][0] = scale[0];
		m[1][1] = scale[1];
		m[2][2] = scale[2];
		m[3][3] = 1;
		return m;
	}

	inline static Mat4f
	mat4_id()
	{
		return mat4_scale(Vec4f{ 1, 1, 1, 1 });
	}

	inline static Mat4f
	mat4_translate(const Vec3f& translation)
	{
		Mat4f m = mat4_id();
		m[3] = Vec4f{translation[0], translation[1], translation[2], 1.0f};
		return m;
	}

	inline static Mat4f
	mat4f_rotate(const Vec3f& axis_normal, float rad)
	{
		Mat4f m = mat4_id();
		float cosine = (float)cos(rad);
		float sine = (float)sin(rad);
		float nx = axis_normal[0];
		float ny = axis_normal[1];
		float nz = axis_normal[2];
		Vec3f temp = axis_normal * (1.0f - cosine);

		// Rodriguries formula
		m[0][0] = nx * temp[0] + cosine;
		m[0][1] = ny * temp[0] + nz * sine;
		m[0][2] = nz * temp[0] - ny * sine;

		m[1][0] = nx * temp[1] - nz * sine;
		m[1][1] = ny * temp[1] + cosine;
		m[1][2] = nz * temp[1] + nx * sine;

		m[2][0] = nx * temp[2] + ny * sine;
		m[2][1] = ny * temp[2] - nx * sine;
		m[2][2] = nz * temp[2] + cosine;

		return m;
	}

	inline static Mat4f
	mat4_transform(const Vec3f axis, float angle_rad, const Vec3f& scale, const Vec3f& translation)
	{
		return mat4_translate(translation) * mat4f_rotate(axis, angle_rad) * mat4_scale(scale);
	}

	inline static Mat4f
	mat4_transpose(const Mat4f& mat)
	{
		Mat4f result{};

		//diagonal
		result[0][0] = mat[0][0];
		result[1][1] = mat[1][1];
		result[2][2] = mat[2][2];
		result[3][3] = mat[3][3];

		//swap cols with rows
		result[0][1] = mat[1][0];
		result[0][2] = mat[2][0];
		result[0][3] = mat[3][0];
		result[1][0] = mat[0][1];
		result[2][0] = mat[0][2];
		result[3][0] = mat[0][3];

		result[1][2] = mat[2][1];
		result[1][3] = mat[3][1];
		result[2][1] = mat[1][2];
		result[3][1] = mat[1][3];

		result[2][3] = mat[3][2];
		result[3][2] = mat[2][3];

		return result;
	}

	inline static float
	mat4_det(const Mat4f& mat)
	{
		//glm
		float sub00 = mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3];
		float sub01 = mat[2][1] * mat[3][3] - mat[3][1] * mat[2][3];
		float sub02 = mat[2][1] * mat[3][2] - mat[3][1] * mat[2][2];
		float sub03 = mat[2][0] * mat[3][3] - mat[3][0] * mat[2][3];
		float sub04 = mat[2][0] * mat[3][2] - mat[3][0] * mat[2][2];
		float sub05 = mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1];

		float det_3x3_00 = (mat[1][1] * sub00 - mat[1][2] * sub01 + mat[1][3] * sub02);
		float det_3x3_01 = -(mat[1][0] * sub00 - mat[1][2] * sub03 + mat[1][3] * sub04);
		float det_3x3_02 = (mat[1][0] * sub01 - mat[1][1] * sub03 + mat[1][3] * sub05);
		float det_3x3_03 = -(mat[1][0] * sub02 - mat[1][1] * sub04 + mat[1][2] * sub05);

		return mat[0][0] * det_3x3_00 + mat[0][1] * det_3x3_01 + mat[0][2] * det_3x3_02 + mat[0][3] * det_3x3_03;
	}

	inline static Mat4f
	mat4_inverse(const Mat4f& mat)
	{
		//glm
		float cof00 = mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3];
		float cof02 = mat[1][2] * mat[3][3] - mat[3][2] * mat[1][3];
		float cof03 = mat[1][2] * mat[2][3] - mat[2][2] * mat[1][3];

		float cof04 = mat[2][1] * mat[3][3] - mat[3][1] * mat[2][3];
		float cof06 = mat[1][1] * mat[3][3] - mat[3][1] * mat[1][3];
		float cof07 = mat[1][1] * mat[2][3] - mat[2][1] * mat[1][3];

		float cof08 = mat[2][1] * mat[3][2] - mat[3][1] * mat[2][2];
		float cof10 = mat[1][1] * mat[3][2] - mat[3][1] * mat[1][2];
		float cof11 = mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2];

		float cof12 = mat[2][0] * mat[3][3] - mat[3][0] * mat[2][3];
		float cof14 = mat[1][0] * mat[3][3] - mat[3][0] * mat[1][3];
		float cof15 = mat[1][0] * mat[2][3] - mat[2][0] * mat[1][3];

		float cof16 = mat[2][0] * mat[3][2] - mat[3][0] * mat[2][2];
		float cof18 = mat[1][0] * mat[3][2] - mat[3][0] * mat[1][2];
		float cof19 = mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2];

		float cof20 = mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1];
		float cof22 = mat[1][0] * mat[3][1] - mat[3][0] * mat[1][1];
		float cof23 = mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1];

		Vec4f fac0{ cof00, cof00, cof02, cof03 };
		Vec4f fac1{ cof04, cof04, cof06, cof07 };
		Vec4f fac2{ cof08, cof08, cof10, cof11 };
		Vec4f fac3{ cof12, cof12, cof14, cof15 };
		Vec4f fac4{ cof16, cof16, cof18, cof19 };
		Vec4f fac5{ cof20, cof20, cof22, cof23 };

		Vec4f vec0{ mat[1][0], mat[0][0], mat[0][0], mat[0][0] };
		Vec4f vec1{ mat[1][1], mat[0][1], mat[0][1], mat[0][1] };
		Vec4f vec2{ mat[1][2], mat[0][2], mat[0][2], mat[0][2] };
		Vec4f vec3{ mat[1][3], mat[0][3], mat[0][3], mat[0][3] };

		//cominors
		Vec4f inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
		Vec4f inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
		Vec4f inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
		Vec4f inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

		//cofactors
		Vec4f s0{ +1, -1, +1, -1 };
		Vec4f s1{ -1, +1, -1, +1 };
		Mat4f inverse{ inv0 * s0, inv1 * s1, inv2 * s0, inv3 * s1 };

		//you already calculated many determinants while inverting, use them to get 1/determinant of the matrix
		Vec4f row0{ inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0] };
		Vec4f dot0 = mat[0] * row0;
		float d = 1 / ((dot0[0] + dot0[1]) + (dot0[2] + dot0[3]));

		return d * inverse;
	}
};