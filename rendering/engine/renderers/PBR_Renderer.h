#pragma once
#pragma once

#include "gpu_gl/glgpu.h"

#include "world/3Dobject.h"
#include "world/Camera.h"

#include <vector>

namespace rndr
{
	struct PBR_Renderer
	{
		glgpu::program prog;
		glgpu::texture irradiance;
		std::vector<const world::object3D*> meshes;
	};

	PBR_Renderer
	pbr_create();

	void
	pbr_free(PBR_Renderer& mr);

	void
	pbr_pack(PBR_Renderer& mr, const world::object3D* mesh);

	void
	pbr_unpack(PBR_Renderer & mr);

	void
	pbr_draw(const PBR_Renderer& mr, const world::Camera& cam);
};