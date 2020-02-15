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
		glgpu::buffer uniform_space;
		glgpu::buffer uniform_object_color;
		glgpu::buffer uniform_light;
		glgpu::buffer uniform_camera;
		glgpu::buffer uniform_material;

		//PBR is based on solving the rendering equation integral, for our case solving the diffuse and specular integrations only (excluding the emission part)
		/* 1) DIFFUSE : we solve the diffuse part of the reflectance integral equation using irradiance convoluted cubemap (diffuse_irradiance_map) 
			that corresponds to the contributions of all incoming light rays from the enviroment map 
			through the hemipshere surrounding a given normal at a given point*/
 		glgpu::cubemap diffuse_irradiance_map;

		/* 2) SPECULAR : we solve the specular part of the reflectance integral equation using split sum approx algorithm by EPIC Games.
			Split sum approx algorithm splits the specular integral part of the reflectance equation to a simpler two integral parts : 
			A) we solve the first part of the specular part of the reflectance integral equation using prefiltered convoluted map that
				corresponds to the contribution of the outcoming reflected rays at a given point of the surface contained by the specular lobe
				which changes according to the surface roughness (specular_prefiltered_map)
			B) we solve the second part of the integral using Lookup Texture of the normal-the incoming light ray angle and surface roughness, 
				which represents the BRDF of the specular integral (specular_BRDF_LUT)
			Read till page 7 for more understanding : https://cdn2.unrealengine.com/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
		*/
		glgpu::cubemap specular_prefiltered_map;
		glgpu::texture specular_BRDF_LUT;

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
	pbr_draw(const PBR_Renderer& self, const world::Camera& camera);
};