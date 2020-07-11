#pragma once

#include "math/Vector.h"

namespace world
{
	struct Material
	{
		math::Vec4f color_norm;
		float metallicity;
		float roughness;
	};
};