#pragma once

#include "math/Matrix.h"

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
	phong_set(Phong self, const world::Camera* camera);

	void
	phong_draw(const Phong self, const math::Mat4f& view_proj, const world::Mesh* mesh, const world::Transform* model, const world::Material* material);
};