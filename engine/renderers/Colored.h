#pragma once
#include "math/Vector.h"

namespace world
{
	struct Camera;
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
	colored_init(Colored self, math::vec2f viewport);

	void
	colored_draw(const Colored self, const world::Camera* camera, const world::Mesh* mesh, const world::Transform* model, math::vec4f& col);
};