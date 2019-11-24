#pragma once
#pragma once

#include "gpu_gl/glgpu.h"

#include "world/3Dobject.h"
#include "world/Camera.h"

namespace rndr
{
	struct PBR_Renderer
	{
		glgpu::program prog;
	};

	PBR_Renderer
	pbr_renderer_create();

	void
	pbr_renderer_free(const PBR_Renderer& mr);

	void
	pbr_renderer_draw(const PBR_Renderer& mr, const world::object3D& object, const world::Camera& cam);
};