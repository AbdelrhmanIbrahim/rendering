#pragma once

#include "gpu_gl/glgpu.h"
#include "geometry/Mesh.h"

namespace rndr
{
	struct Mesh_Renderer
	{
		glgpu::program prog;
	};

	Mesh_Renderer
	mesh_renderer_create();

	void
	mesh_renderer_free(const Mesh_Renderer& mr);

	void
	mesh_rendere_draw(const Mesh_Renderer& mr, const geo::Mesh& mesh);
};