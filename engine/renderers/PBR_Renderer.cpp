#include "PBR_Renderer.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

using namespace glgpu;
using namespace math;
using namespace world;

namespace rndr
{
	struct Space_Uniform
	{
		Mat4f model;
		Mat4f vp;
	};

	struct Light_Uniform
	{
		vec4f color;
		vec4f dir;
	};

	struct Camera_Uniform
	{
		vec4f camera_world_pos;
	};

	struct Material_Uniform
	{
		float metallicity;
		float roughness;
	};

	PBR_Renderer
	pbr_create()
	{
		PBR_Renderer self{};

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self.prog = program_create("F:/Abdo/rendering_jo/rendering/engine/shaders/pbr.vertex", "F:/Abdo/rendering_jo/rendering/engine/shaders/pbr.pixel");
		self.uniform_space = buffer_uniform_create(sizeof(Space_Uniform));
		self.uniform_object_color = buffer_uniform_create(sizeof(vec4f));
		self.uniform_light = buffer_uniform_create(sizeof(Light_Uniform));
		self.uniform_camera = buffer_uniform_create(sizeof(Camera_Uniform));
		self.uniform_material = buffer_uniform_create(sizeof(Material_Uniform));

		/*Diffuse irriadiance convoluted map*/
		io::Image diff = image_read("F:/Abdo/rendering_jo/rendering/res/imgs/hdr/Tokyo_diff.hdr", io::IMAGE_FORMAT::HDR);
		self.diffuse_irradiance_map = cubemap_hdr_create(diff, vec2f{512, 512}, false);
		image_free(diff);

		//Specular prefiltered convoluted map (Part 1 from the specular integration of the reflectance equation)
		//1) create prefiltered convolution map with mipmap generated, rougher = blurrier reflections
		//2) create env_cmap that will be used for convolution to create prefilterd map
		//3) convolute env_cmap 5 times (mipmaps) using prefiltering shader and save them to specular_prefiltered_map mipmaps, diff roughness = diff prefiltered map
		vec2f prefiltered_initial_size{ 128, 128 };
		self.specular_prefiltered_map = cubemap_create(prefiltered_initial_size, INTERNAL_TEXTURE_FORMAT::RGB16F, EXTERNAL_TEXTURE_FORMAT::RGB, DATA_TYPE::FLOAT, true);

		io::Image env = image_read("F:/Abdo/rendering_jo/rendering/res/imgs/hdr/Tokyo_spec.hdr", io::IMAGE_FORMAT::HDR);
		cubemap env_cmap = cubemap_hdr_create(env, vec2f{ 512, 512 }, true);

		program prefiltering_prog = program_create("F:/Abdo/rendering_jo/rendering/engine/shaders/cube.vertex", "F:/Abdo/rendering_jo/rendering/engine/shaders/specular_prefiltering_convolution.pixel");
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
		program BRDF_prog = program_create("F:/Abdo/rendering_jo/rendering/engine/shaders/quad_ndc.vertex", "F:/Abdo/rendering_jo/rendering/engine/shaders/specular_BRDF_convolution.pixel");
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
		buffer_delete(self.uniform_space);
		buffer_delete(self.uniform_object_color);
		buffer_delete(self.uniform_light);
		buffer_delete(self.uniform_camera);
		buffer_delete(self.uniform_material);
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
	pbr_draw(const PBR_Renderer& self, const Camera& camera)
	{
		color_clear(0.1f, 0.1f, 0.1f);
		program_use(self.prog);

		//viewport
		vec2f viewport = world::camera_viewport(camera);
		view_port(0, 0, (int)viewport[0], (int)viewport[1]);

		//uniform blocks
		buffer_uniform_bind(0, self.uniform_space);
		buffer_uniform_bind(1, self.uniform_object_color);
		buffer_uniform_bind(2, self.uniform_light);
		buffer_uniform_bind(3, self.uniform_camera);
		buffer_uniform_bind(4, self.uniform_material);

		//till we get sampler objects in
		cubemap_bind(self.diffuse_irradiance_map, TEXTURE_UNIT::UNIT_0);
		uniform1i_set(self.prog, "diffuse_irradiance_map", TEXTURE_UNIT::UNIT_0);
		cubemap_bind(self.specular_prefiltered_map, TEXTURE_UNIT::UNIT_1);
		uniform1i_set(self.prog, "specular_prefiltered_map", TEXTURE_UNIT::UNIT_1);
		texture2d_bind(self.specular_BRDF_LUT, TEXTURE_UNIT::UNIT_2);
		uniform1i_set(self.prog, "specular_BRDF_LUT", TEXTURE_UNIT::UNIT_2);

		for (auto object : self.meshes)
		{
			//uniform blocks
			Space_Uniform mvp{ mat4_from_transform(object->model), camera_view_proj(camera) };
			buffer_uniform_set(self.uniform_space, &mvp, sizeof(mvp));
			vec4f color_test{ 0.75f, 0.75f, 0.75f, 1.0f };
			buffer_uniform_set(self.uniform_object_color, &color_test, sizeof(color_test));
			Light_Uniform light{ vec4f{ 1.0f, 1.0f, 1.0f,1.0f }, vec4f{ 0.0f, -1.0f, 0.0f, 0.0f } };
			buffer_uniform_set(self.uniform_light, &light, sizeof(light));
			Camera_Uniform cam{ camera.pos[0], camera.pos[1], camera.pos[2], 0.0f };
			buffer_uniform_set(self.uniform_camera, &cam, sizeof(cam));
			Material_Uniform mat{ 0.8, 0.2};
			buffer_uniform_set(self.uniform_material, &mat, sizeof(mat));

			//draw geometry
			vao_bind(object->mesh.va, object->mesh.vs, object->mesh.is);
			draw_indexed(object->mesh.indices.size());
			vao_unbind();
		}
	}
}