#pragma once

#include "gpu_gl/glgpu.h"

#include "world/3Dobject.h"

#include "math/Vector.h"

namespace rndr
{
	struct Mesh_Renderer
	{
		glgpu::program prog;
		glgpu::texture tex;
	};

	Mesh_Renderer
	mesh_renderer_create();

	void
	mesh_renderer_free(const Mesh_Renderer& mr);

	void
	mesh_renderer_draw(const Mesh_Renderer& mr, const world::object3D& object, const math::vec2f& viewport);
};