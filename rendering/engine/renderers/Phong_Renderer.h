#pragma once

#include "gpu_gl/glgpu.h"

#include "world/3Dobject.h"
#include "world/Camera.h"

namespace rndr
{
	struct Phong_Renderer
	{
		glgpu::program prog;
	};

	Phong_Renderer
	phong_renderer_create();

	void
	phong_renderer_free(const Phong_Renderer& mr);

	void
	phong_renderer_draw(const Phong_Renderer& mr, const world::object3D& object, const world::Camera& cam);
};