#pragma once

#include "world/3Dobject.h"
#include "world/Camera.h"

namespace rndr
{
	typedef struct IPBR* PBR;

	PBR
	pbr_create();

	void
	pbr_free(PBR self);

	void
	pbr_pack(PBR self, const world::object3D* mesh);

	void
	pbr_unpack(PBR self);

	void
	pbr_draw(const PBR self, const world::Camera& camera);
};