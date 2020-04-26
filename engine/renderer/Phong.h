#pragma once

#include "math/Matrix.h"
#include "world/component/Sun.h"
#include "world/component/Lamp.h"
#include "world/component/Flash.h"

#include "infra/mem/chunck.h"

#include <vector>

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
	phong_set(Phong self,
		world::Camera* camera,
		infra::mem::chunk<world::Sun> suns,
		infra::mem::chunk<world::Lamp> lamps,
		infra::mem::chunk<world::Flash> flashes);

	void
	phong_draw(const Phong self, 
		const math::Mat4f& view_proj, 
		const world::Mesh* mesh, 
		const world::Transform* model, 
		const world::Material* material);
};