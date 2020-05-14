#pragma once

#include "math/Vector.h"

namespace world
{
	struct TVertex
	{
		math::vec3f pos;
		math::vec3f normal;
		math::vec2f uv;
	};

	struct CVertex
	{
		math::vec3f pos;
		math::vec4f col;
	};
};