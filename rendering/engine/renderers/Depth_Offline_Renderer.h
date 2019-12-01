#pragma once

#include "gpu_gl/glgpu.h"

#include "world/3Dobject.h"
#include "world/Camera.h"

#include <vector>

namespace rndr
{
	struct Depth_Offline_Renderer
	{
		glgpu::framebuffer fb;
		glgpu::texture depth;
		glgpu::program prog;
		std::vector<const world::object3D*> meshes;
	};

	Depth_Offline_Renderer
	depth_offline_create();

	void
	depth_offline_free(Depth_Offline_Renderer& mr);

	void
	depth_offline_pack(Depth_Offline_Renderer& mr, const world::object3D* mesh);

	void
	depth_offline_unpack(Depth_Offline_Renderer & mr);

	glgpu::texture
	depth_offline_draw(const Depth_Offline_Renderer& mr, const math::Mat4f& view_proj);
};