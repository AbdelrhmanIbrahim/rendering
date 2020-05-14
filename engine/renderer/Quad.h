#pragma once

#include "math/Matrix.h"
#include "math/Vector.h"

#include "world/component/Vertex.h"

namespace rndr
{
	typedef struct ICquad* Cquad;

	Cquad
	cquad_create();

	void
	cquad_free(Cquad self);

	void
	cquad_set(Cquad self, math::Mat4f& view_proj, math::vec2f& viewport);

	void
	cquad_draw(Cquad self, const world::CVertex quad_strip[6]);
};