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
	typedef struct IPBR* PBR;

	PBR
	pbr_create();

	void
	pbr_free(PBR self);

	void
	pbr_init(PBR self, math::vec2f viewport);

	void
	pbr_draw(PBR self, const world::Camera* camera, const world::Mesh* mesh, const world::Transform* model, const world::Material* material);
};