#pragma once

namespace math
{
	struct vec3f
	{
		float pos[3];
	};

	inline bool
	operator==(const vec3f& first, const vec3f& second)
	{
		return first.pos[0] == second.pos[0] && first.pos[1] == second.pos[1] && first.pos[2] == second.pos[2];
	}
}