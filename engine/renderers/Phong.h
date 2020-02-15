#pragma once

#include "world/3Dobject.h"
#include "world/Camera.h"

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
};