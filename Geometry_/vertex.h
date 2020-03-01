#pragma once

#include "math/vector.h"

namespace geo
{
	struct Vertex
	{
		math::vec3f pos;
		math::vec3f normal;
		math::vec2f uv;
	};
};