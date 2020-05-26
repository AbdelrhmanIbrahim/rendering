#pragma once

#include "math/Matrix.h"
#include "math/Vector.h"

#include "world/component/Vertex.h"

#include "gl/glgpu.h"

namespace rndr
{
	//Textured quad
	typedef struct ITQuad* TQuad;

	TQuad
	tquad_create();

	void
	tquad_free(TQuad self);

	void
	tquad_set(TQuad self, const glgpu::Texture texture, math::Mat4f& view_proj, math::vec2f& viewport);

	void
	tquad_draw(TQuad self, const world::TVertex quad_strip[6]);
};