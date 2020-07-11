#pragma once

#include "math/Vector.h"

namespace world
{
	struct CVertex
	{
		math::Vec3f pos;
		math::Vec4f col;
	};

	struct TVertex
	{
		math::Vec3f pos;
		math::Vec3f normal;
		math::Vec2f uv;
	};
};