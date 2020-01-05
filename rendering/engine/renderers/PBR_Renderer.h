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
		/*the diffuse part of the reflectance integral equation : irradiance convoluted cubemap that corresponds to the contribution of
		all incoming light rays from the enviroment map through the hemipshere surronding a given point*/
 		glgpu::cubemap diffuse_irradiance_map;
		/*the first part of the specular part of the reflectance integral equation : prefiltered convoluted map that corresponds to the contribution of
		the outcoming reflected rays contained by the specular lobe according to the surface roughness at a given point*/
		glgpu::cubemap specular_prefiltered_map;
		std::vector<const world::object3D*> meshes;
	};

	PBR_Renderer
	pbr_create();

	void
	pbr_free(PBR_Renderer& self);

	void
	pbr_pack(PBR_Renderer& self, const world::object3D* mesh);

	void
	pbr_unpack(PBR_Renderer & self);

	void
	pbr_draw(const PBR_Renderer& self, const world::Camera& cam);
};