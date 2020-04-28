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
	point_append(Point self, const math::vec3f& pos, const math::vec4f& col);

	void
	point_set(Point self, const math::Mat4f& view_proj);

	void
	point_draw(Point self);
};