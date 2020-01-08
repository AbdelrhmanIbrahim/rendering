#include "PBR_Renderer.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

using namespace glgpu;
using namespace math;
using namespace world;

namespace rndr
{
	PBR_Renderer
	pbr_create()
	{
		PBR_Renderer self{};

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self.prog = program_create("../rendering/engine/shaders/pbr.vertex", "../rendering/engine/shaders/pbr.pixel");

		/*Diffuse irriadiance convoluted map*/
		io::Image diff = image_read("../rendering/res/imgs/hdr/Tokyo_diff.hdr", io::IMAGE_FORMAT::HDR);
		self.diffuse_irradiance_map = cubemap_hdr_create(diff, vec2f{512, 512}, false);
		image_free(diff);

		//Specular prefiltered convoluted map (Part 1 from the specular integration of the reflectance equation)
		//1) create prefiltered convolution map with mipmap generated, rougher = blurrier reflections
		//2) create env_cmap that will be used for convolution to create prefilterd map
		//3) convolute env_cmap 5 times (mipmaps) using prefiltering shader and save them to specular_prefiltered_map mipmaps, diff roughness = diff prefiltered map
		vec2f prefiltered_initial_size{ 128, 128 };
		self.specular_prefiltered_map = cubemap_create(prefiltered_initial_size, INTERNAL_TEXTURE_FORMAT::RGB16F, EXTERNAL_TEXTURE_FORMAT::RGB, DATA_TYPE::FLOAT, true);

		io::Image env = image_read("../rendering/res/imgs/hdr/Tokyo_spec.hdr", io::IMAGE_FORMAT::HDR);
		cubemap env_cmap = cubemap_hdr_create(env, vec2f{ 512, 512 }, false);

		program prefiltering_prog = program_create("../rendering/engine/shaders/cube.vertex", "../rendering/engine/shaders/specular_prefiltering_convolution.pixel");
		unsigned int max_mipmaps = 5;
		for (unsigned int mip_level = 0; mip_level < max_mipmaps; ++mip_level)
		{
			float roughness = (float)mip_level / max_mipmaps;
			vec2f mipmap_size{ prefiltered_initial_size[0] * std::pow(0.5, mip_level) , prefiltered_initial_size[0] * std::pow(0.5, mip_level) };
			cubemap_postprocess(env_cmap, self.specular_prefiltered_map, prefiltering_prog, Unifrom_Float{"roughness", roughness}, mipmap_size, mip_level);
		}
		program_delete(prefiltering_prog);
		cubemap_free(env_cmap);
		image_free(env);

		//Specular BRDF convoluted LUT (Part 2 from the specular integration of the reflectance equation)
		program BRDF_prog = program_create("../rendering/engine/shaders/quad_ndc.vertex", "../rendering/engine/shaders/specular_BRDF_convolution.pixel");
		vec2f BRDF_LUT_size{ 512, 512 };
		self.specular_BRDF_LUT = texture2d_create(BRDF_LUT_size, INTERNAL_TEXTURE_FORMAT::RG16F, EXTERNAL_TEXTURE_FORMAT::RG, DATA_TYPE::FLOAT, false);
		texture2d_render_offline_to(self.specular_BRDF_LUT, BRDF_prog, BRDF_LUT_size);
		program_delete(BRDF_prog);

		return self;
	}

	void
	pbr_free(PBR_Renderer & self)
	{
		program_delete(self.prog);
		cubemap_free(self.diffuse_irradiance_map);
		cubemap_free(self.specular_prefiltered_map);
	}
	
	void
	pbr_pack(PBR_Renderer& self, const world::object3D* mesh)
	{
		self.meshes.push_back(mesh);
	}

	void
	pbr_unpack(PBR_Renderer & self)
	{
		self.meshes.clear();
	}

	void
	pbr_draw(const PBR_Renderer& self, const Camera& cam)
	{
		color_clear(0.1f, 0.1f, 0.1f);
		program_use(self.prog);

		//viewport
		vec2f viewport = world::camera_viewport(cam);
		view_port(0, 0, (int)viewport[0], (int)viewport[1]);

		//uniforms
		uniform3f_set(self.prog, "camera_pos_world", cam.pos);
		uniform3f_set(self.prog, "light_color", vec3f{ 1.0f, 1.0f, 1.0f });
		uniform3f_set(self.prog, "light_dir", vec3f{ 0.0f, -1.0f, 0.0f });
		
		cubemap_bind(self.diffuse_irradiance_map, TEXTURE_UNIT::UNIT_0);
		uniform1i_set(self.prog, "diffuse_irradiance_map", TEXTURE_UNIT::UNIT_0);
		cubemap_bind(self.specular_prefiltered_map, TEXTURE_UNIT::UNIT_1);
		uniform1i_set(self.prog, "specular_prefiltered_map", TEXTURE_UNIT::UNIT_1);
		for (const auto object : self.meshes)
		{
			//MVP
			Mat4f model = mat4_from_transform(object->model);
			Mat4f mvp = camera_view_proj(cam) * model;

			uniformmat4f_set(self.prog, "mvp", mvp);
			uniformmat4f_set(self.prog, "model", model);
			uniform3f_set(self.prog, "object_color_albedo", vec3f{ 0.75f, 0.75f, 0.75f});
			uniform1f_set(self.prog, "metallic", 0.9);
			uniform1f_set(self.prog, "rough", 0.2);

			//draw geometry
			vao_bind(object->mesh.va, object->mesh.vs, object->mesh.is);
			draw_indexed(object->mesh.indices.size());
			vao_unbind();
		}
	}
}