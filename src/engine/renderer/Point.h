#pragma once
#include "math/Vector.h"
#include "math/Matrix.h"

#include "world/component/Vertex.h"

namespace rndr
{
	typedef struct IPoint* Point;

	Point
	point_create();

	void
	point_free(Point self);

	void
	point_append(Point self, world::CVertex vertex);

	void
	point_set(Point self, const math::Mat4f& view_proj);

	void
	point_draw(Point self);
};