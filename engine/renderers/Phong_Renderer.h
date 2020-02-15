#pragma once

#include "gpu_gl/glgpu.h"

#include "world/3Dobject.h"
#include "world/Camera.h"

#include <vector>

namespace rndr
{
	struct Phong_Renderer
	{
		glgpu::program prog;
		glgpu::buffer uniform_object_color;
		std::vector<const world::object3D*> meshes;
	};

	Phong_Renderer
	phong_create();

	void
	phong_free(Phong_Renderer& mr);

	void
	phong_pack(Phong_Renderer& mr, const world::object3D* mesh);

	void
	phong_unpack(Phong_Renderer & mr);

	void
	phong_draw(const Phong_Renderer& mr, const world::Camera& cam);
};