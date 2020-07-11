#pragma once

namespace math
{
	struct Vec2f
	{
		float data[2];

		inline float&
		operator[](unsigned int index)
		{
			return data[index];
		}

		inline const float&
		operator[](unsigned int index) const
		{
			return data[index];
		}

		inline Vec2f
		operator*(float factor) const
		{
			return Vec2f{ data[0] * factor, data[1] * factor};
		}
	};

	struct Vec3f
	{
		float data[3];

		inline float&
		operator[](unsigned int index)
		{
			return data[index];
		}
		
		inline const float&
		operator[](unsigned int index) const
		{
			return data[index];
		}

		inline Vec3f
		operator+(const Vec3f& other) const
		{
			return Vec3f{ data[0] + other.data[0], data[1] + other.data[1], data[2] + other.data[2] };
		}

		inline Vec3f
		operator-(const Vec3f& other) const
		{
			return Vec3f{ data[0] - other.data[0], data[1] - other.data[1], data[2] - other.data[2] };
		}

		inline Vec3f
		operator-() const
		{
			return Vec3f{ -data[0], -data[1], -data[2]};
		}

		inline Vec3f
		operator*(const Vec3f& other) const
		{
			return Vec3f{ other.data[0]*data[0], other.data[1]*data[1], other.data[2]*data[2] };
		}

		inline Vec3f&
		operator-=(const Vec3f& vec)
		{
			data[0] -= vec[0];
			data[1] -= vec[1];
			data[2] -= vec[2];
			return *this;
		}

		inline Vec3f&
		operator+=(const Vec3f& vec)
		{
			data[0] += vec[0];
			data[1] += vec[1];
			data[2] += vec[2];
			return *this;
		}

		inline Vec3f
		operator*(const float factor) const
		{
			return Vec3f{ factor*data[0], factor*data[1], factor*data[2] };
		}

		inline Vec3f
		operator/(const float factor) const
		{
			return Vec3f{ data[0]/factor, data[1]/factor, data[2]/factor };
		}

		inline Vec3f&
		operator*=(const float factor)
		{
			data[0] *= factor;
			data[1] *= factor;
			data[2] *= factor;
			return *this;
		}

		inline Vec3f&
		operator+=(const float factor)
		{
			data[0] += factor;
			data[1] += factor;
			data[2] += factor;
			return *this;
		}
	};

	struct Vec4f
	{
		float data[4];

		inline float&
		operator[](unsigned int index)
		{
			return data[index];
		}

		inline const float&
		operator[](unsigned int index) const
		{
			return data[index];
		}

		inline Vec4f
		operator+(const Vec4f& other) const
		{
			return Vec4f{ data[0] + other.data[0], data[1] + other.data[1], data[2] + other.data[2], data[3] + other.data[3] };
		}

		inline Vec4f
		operator-(const Vec4f& other) const
		{
			return Vec4f{ data[0] - other.data[0], data[1] - other.data[1], data[2] - other.data[2], data[3] - other.data[3] };
		}

		inline Vec4f
		operator-() const
		{
			return Vec4f{ -data[0], -data[1], -data[2], -data[3] };
		}

		inline Vec4f
		operator*(const Vec4f& other) const
		{
			return Vec4f{ data[0] * other.data[0], data[1] * other.data[1], data[2] * other.data[2], data[3] * other.data[3] };
		}

		inline Vec4f
		operator*(const float factor) const
		{
			return Vec4f{ factor*data[0], factor*data[1], factor*data[2], factor*data[3] };
		}

		inline Vec4f
		operator/(const float factor) const
		{
			return Vec4f{ data[0] / factor, data[1] / factor, data[2] / factor, data[3]/factor };
		}

		inline Vec4f&
		operator*=(const float factor)
		{
			data[0] *= factor;
			data[1] *= factor;
			data[2] *= factor;
			data[3] *= factor;
			return *this;
		}

		inline Vec4f&
		operator+=(const float factor)
		{
			data[0] += factor;
			data[1] += factor;
			data[2] += factor;
			data[3] += factor;
			return *this;
		}
	};

	//lhs
	inline bool
	operator==(const Vec2f& first, const Vec2f& second)
	{
		return first.data[0] == second.data[0] && first.data[1] == second.data[1];
	}

	inline bool
	operator!=(const Vec2f& first, const Vec2f& second)
	{
		return first.data[0] != second.data[0] || first.data[1] != second.data[1];
	}

	inline bool
	operator==(const Vec3f& first, const Vec3f& second)
	{
		return first.data[0] == second.data[0] && first.data[1] == second.data[1] && first.data[2] == second.data[2];
	}

	inline bool
	operator!=(const Vec3f& first, const Vec3f& second)
	{
		return first.data[0] != second.data[0] || first.data[1] != second.data[1] || first.data[2] != second.data[2];
	}

	inline bool
	operator==(const Vec4f& first, const Vec4f& second)
	{
		return first.data[0] == second.data[0] && first.data[1] == second.data[1] && first.data[2] == second.data[2] && first.data[3] == second.data[3];
	}

	inline bool
	operator!=(const Vec4f& first, const Vec4f& second)
	{
		return first.data[0] != second.data[0] || first.data[1] != second.data[1] || first.data[2] != second.data[2] || first.data[3] != second.data[3];
	}

	inline Vec2f
	operator*(float factor, const Vec2f& vec)
	{
		return vec * factor;
	}

	inline Vec3f
	operator*(float factor, const Vec3f& vec)
	{
		return vec * factor;
	}

	inline Vec4f
	operator*(float factor, const Vec4f& vec)
	{
		return vec * factor;
	}

	constexpr Vec3f X_AXIS = Vec3f{ 1.0f, 0.0f, 0.0f };
	constexpr Vec3f Y_AXIS = Vec3f{ 0.0f, 1.0f, 0.0f };
	constexpr Vec3f Z_AXIS = Vec3f{ 0.0f, 0.0f, 1.0f };
}