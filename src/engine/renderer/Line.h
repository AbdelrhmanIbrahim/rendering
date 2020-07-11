#pragma once
#include "math/Vector.h"
#include "math/Matrix.h"

#include "world/component/Vertex.h"

namespace rndr
{
	typedef struct ILine* Line;

	Line
	line_create();

	void
	line_free(Line self);

	void
	line_append(Line self, world::CVertex p0, world::CVertex p1);

	void
	line_set(Line self, const math::Mat4f& view_proj);

	void
	line_draw(Line self);
};