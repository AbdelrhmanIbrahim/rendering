#pragma once

#include "world/components/Camera.h"
#include "world/components/Mesh.h"
#include "world/components/Transform.h"

namespace rndr
{
	typedef struct IPhong* Phong;

	Phong
	phong_create();

	void
	phong_free(Phong self);

	void
	_phong_draw(const Phong self, const world::Camera& camera, const world::Mesh& mesh, const world::Transform& transform);
};