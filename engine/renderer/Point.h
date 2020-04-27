#pragma once
#include "math/Vector.h"
#include "math/Matrix.h"

namespace rndr
{
	typedef struct IPoint* Point;

	Point
	point_create();

	void
	point_free(Point self);

	void
	point_append(Point self, math::vec3f& point);

	void
	point_set(Point self, const math::Mat4f& view_proj, math::vec4f& col);

	void
	point_draw(Point self);
};