#pragma once

#include "math/Matrix.h"
#include "math/Vector.h"

#include "world/component/Vertex.h"

namespace rndr
{
	typedef struct IQuad* Quad;

	Quad
	quad_create();

	void
	quad_free(Quad self);

	void
	quad_set(Quad self, math::Mat4f& view_proj, math::vec2f& viewport);

	void
	quad_draw(Quad self, const world::CVertex quad_strip[6]);
};