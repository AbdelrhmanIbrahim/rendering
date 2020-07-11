#pragma once

#include "math/Matrix.h"
#include "math/Vector.h"

#include "world/component/Vertex.h"

namespace rndr
{
	//colored quad
	typedef struct ICQuad* CQuad;

	CQuad
	cquad_create();

	void
	cquad_free(CQuad self);

	void
	cquad_set(CQuad self, math::Mat4f& view_proj, math::vec2f& viewport);

	void
	cquad_draw(CQuad self, const world::CVertex quad_strip[6]);
};