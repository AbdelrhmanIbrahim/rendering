#pragma once
#include "math/Vector.h"
#include "math/Matrix.h"

namespace world
{
	struct Mesh;
	struct Transform;
};

namespace rndr
{
	typedef struct IHiddenline* Hiddenline;

	Hiddenline
	hiddenline_create();

	void
	hiddenline_free(Hiddenline self);

	void
	hiddenline_set(Hiddenline self, math::Vec2f viewport);

	void
	hiddenline_draw(Hiddenline self, const math::Mat4f& view_proj, const world::Mesh* mesh, const world::Transform* model, const math::Vec4f& col);
};