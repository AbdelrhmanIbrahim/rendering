#pragma once

namespace math
{
	struct vec2f
	{
		float data[2];

		float&
		operator[](unsigned int index)
		{
			return data[index];
		}
	};

	struct vec3f
	{
		float data[3];

		float&
		operator[](unsigned int index)
		{
			return data[index];
		}

		vec3f
		operator+(const vec3f& other) const
		{
			return vec3f{ data[0] + other.data[0], data[1] + other.data[1], data[2] + other.data[2] };
		}

		vec3f
		operator-(const vec3f& other) const
		{
			return vec3f{ data[0] - other.data[0], data[1] - other.data[1], data[2] - other.data[2] };
		}

		vec3f
		operator*(const vec3f& other) const
		{
			return vec3f{ other.data[0]*data[0], other.data[1]*data[1], other.data[2]*data[2] };
		}

		vec3f 
		operator*(const float factor) const
		{
			return vec3f{ factor*data[0], factor*data[1], factor*data[2] };
		}

		vec3f
		operator/(const float factor) const
		{
			return vec3f{ data[0]/factor, data[1]/factor, data[2]/factor };
		}

		vec3f&
		operator*=(const float factor)
		{
			data[0] *= factor;
			data[1] *= factor;
			data[2] *= factor;
			return *this;
		}

		vec3f&
		operator+=(const float factor)
		{
			data[0] += factor;
			data[1] += factor;
			data[2] += factor;
			return *this;
		}
	};

	struct vec4f
	{
		float data[4];

		float&
		operator[](unsigned int index)
		{
			return data[index];
		}

		vec4f
		operator+(const vec3f& other) const
		{
			return vec4f{ data[0] + other.data[0], data[1] + other.data[1], data[2] + other.data[2], data[3] + other.data[3] };
		}

		vec4f
		operator-(const vec3f& other) const
		{
			return vec4f{ data[0] - other.data[0], data[1] - other.data[1], data[2] - other.data[2], data[3] - other.data[3] };
		}

		vec4f
		operator*(const vec3f& other) const
		{
			return vec4f{ data[0] * other.data[0], data[1] * other.data[1], data[2] * other.data[2], data[3] * other.data[3] };
		}

		vec4f
		operator*(const float factor) const
		{
			return vec4f{ factor*data[0], factor*data[1], factor*data[2], factor*data[3] };
		}

		vec4f
		operator/(const float factor) const
		{
			return vec4f{ data[0] / factor, data[1] / factor, data[2] / factor, data[3]/factor };
		}

		vec4f&
		operator*=(const float factor)
		{
			data[0] *= factor;
			data[1] *= factor;
			data[2] *= factor;
			data[3] *= factor;
			return *this;
		}

		vec4f&
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
	operator==(const vec3f& first, const vec3f& second)
	{
		return first.data[0] == second.data[0] && first.data[1] == second.data[1] && first.data[2] == second.data[2];
	}

	inline bool
	operator==(const vec4f& first, const vec4f& second)
	{
		return first.data[0] == second.data[0] && first.data[1] == second.data[1] && first.data[2] == second.data[2] && first.data[3] == second.data[3];
	}
}