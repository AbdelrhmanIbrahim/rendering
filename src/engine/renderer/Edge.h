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
	typedef struct IEdge* Edge;

	Edge
	edge_create();

	void
	edge_free(Edge self);

	void
	edge_set(Edge self, math::Vec2f viewport);

	void
	edge_draw(Edge self, const math::Mat4f& view_proj, const world::Mesh* mesh, const world::Transform* model, const math::Vec4f& col);
};