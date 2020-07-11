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
	typedef struct IColored* Colored;

	Colored
	colored_create();

	void
	colored_free(Colored self);

	void
	colored_set(Colored self, math::Vec2f viewport);

	void
	colored_draw(const Colored self, const math::Mat4f& view_proj, const world::Mesh* mesh, const world::Transform* model, const math::Vec4f& col);
};