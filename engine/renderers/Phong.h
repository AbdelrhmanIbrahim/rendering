#pragma once

#include "world/3Dobject.h"
#include "world/Camera.h"

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
	phong_pack(Phong self, const world::object3D* mesh);

	void
	phong_unpack(Phong self);

	void
	phong_draw(const Phong self , const world::Camera& cam);

	void
	_phong_draw(const Phong self, const world::_Camera& camera, const world::_Mesh& mesh, const world::_Transform& transform);
};