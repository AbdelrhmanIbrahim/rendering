#pragma once

namespace math
{
	struct vec2f
	{
		float data[2];
	};

	struct vec3f
	{
		float data[3];
	};

	struct vec4f
	{
		float data[4];

		float 
		operator[](unsigned int index)
		{
			return data[index];
		}
	};

	inline bool
	operator==(const vec3f& first, const vec3f& second)
	{
		return first.data[0] == second.data[0] && first.data[1] == second.data[1] && first.data[2] == second.data[2];
	}
}