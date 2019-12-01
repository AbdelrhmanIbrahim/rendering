#pragma once

#include "gpu_gl/glgpu.h"

#include "world/3Dobject.h"
#include "world/Camera.h"

#include "math/Vector.h"

#include <vector>

namespace rndr
{
	struct Phong_Shadow_Renderer
	{
		glgpu::framebuffer fb;
		glgpu::texture depth;
		glgpu::program depth_prog;
		glgpu::program phong_shadow_prog;
		std::vector<const world::object3D*> meshes;
	};

	Phong_Shadow_Renderer
	phong_shadow_create();

	void
	phong_shadow_free(Phong_Shadow_Renderer& mr);

	void
	phong_shadow_pack(Phong_Shadow_Renderer& mr, const world::object3D* mesh);

	void
	phong_shadow_unpack(Phong_Shadow_Renderer & mr);

	void
	phong_shadow_draw(const Phong_Shadow_Renderer& mr, const math::vec3f& light_pos, const world::Camera& viewer);
};