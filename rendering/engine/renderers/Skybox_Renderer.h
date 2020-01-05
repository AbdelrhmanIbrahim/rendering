#pragma once

#include "gpu_gl/glgpu.h"

#include "world/3Dobject.h"
#include "world/Camera.h"

#include "IO/Image.h"

namespace rndr
{
	struct Skybox_Renderer
	{
		glgpu::program prog;
		glgpu::vao cube;
		glgpu::buffer cube_vs;
		glgpu::cubemap cubemap;
	};

	Skybox_Renderer
	skybox_renderer_rgba_create(const char** skybox_imgs_paths, io::IMAGE_FORMAT format);

	Skybox_Renderer
	skybox_renderer_hdr_create(const char* skybox_hdr_path);

	void
	skybox_renderer_free(const Skybox_Renderer& sbr);

	void
	skybox_renderer_draw(const Skybox_Renderer& sbr, const world::Camera& cam);
};