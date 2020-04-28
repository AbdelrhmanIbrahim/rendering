#pragma once
#include "math/Vector.h"
#include "math/Matrix.h"

#include "defs/Defs.h"

namespace rndr
{
	typedef struct ILine* Line;

	Line
	line_create();

	void
	line_free(Line self);

	void
	line_append(Line self, Pnt p0, Pnt p1);

	void
	line_set(Line self, const math::Mat4f& view_proj);

	void
	line_draw(Line self);
};