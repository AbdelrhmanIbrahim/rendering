#pragma once

#include "math/Vector.h"

namespace world
{
	struct Camera;
	struct Mesh;
	struct Transform;
	struct Material;
};

namespace rndr
{
	typedef struct IPhong* Phong;

	Phong
	phong_create();

	void
	phong_free(Phong self);

	void
	phong_init(Phong self, math::vec2f viewport);

	void
	phong_draw(const Phong self, const world::Camera* camera, const world::Mesh* mesh, const world::Transform* model, const world::Material* material);
};