#pragma once

#include "gpu_gl/glgpu.h"

#include "world/3Dobject.h"
#include "world/Camera.h"

namespace rndr
{
	struct Skybox_Renderer
	{
		glgpu::program prog;
		glgpu::vao cube;
		glgpu::buffer cube_vs;
		glgpu::texture cubemap;
	};

	Skybox_Renderer
	skybox_renderer_create(const char** skybox_paths);

	void
	skybox_renderer_free(const Skybox_Renderer& sbr);

	void
	skybox_renderer_draw(const Skybox_Renderer& sbr, const world::Camera& cam);
};