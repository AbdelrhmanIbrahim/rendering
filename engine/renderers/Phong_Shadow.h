#pragma once

#include "world/3Dobject.h"
#include "world/Camera.h"

#include "math/Vector.h"

namespace rndr
{
	typedef struct IPhong_Shadow* Phong_Shadow;

	Phong_Shadow
	phong_shadow_create();

	void
	phong_shadow_free(Phong_Shadow mr);

	void
	phong_shadow_pack(Phong_Shadow mr, const world::object3D* mesh);

	void
	phong_shadow_unpack(Phong_Shadow mr);

	void
	phong_shadow_draw(const Phong_Shadow mr, const math::vec3f& light_pos, const world::Camera& viewer);
};