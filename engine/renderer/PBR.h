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
	typedef struct IPBR* PBR;

	PBR
	pbr_create();

	void
	pbr_free(PBR self);

	void
	pbr_set(PBR self, const world::Camera* cam);

	void
	pbr_draw(PBR self, const math::Mat4f& view_proj, const world::Mesh* mesh, const world::Transform* model, const world::Material* material);
};