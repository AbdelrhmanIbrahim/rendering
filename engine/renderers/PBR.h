#pragma once

#include "world/components/Camera.h"
#include "world/components/Mesh.h"
#include "world/components/Transform.h"

namespace rndr
{
	typedef struct IPBR* PBR;

	PBR
	pbr_create();

	void
	pbr_free(PBR self);

	void
	pbr_draw(const PBR self, const world::Camera& camera, const world::Mesh& mesh, const world::Transform& model);
};