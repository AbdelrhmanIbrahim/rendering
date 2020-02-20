#include "PBR.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

#include "gpu_gl/glgpu.h"

#include <vector>

using namespace glgpu;
using namespace math;
using namespace world;

namespace rndr
{
	struct IPBR
	{
		glgpu::Program prog;
		glgpu::Buffer uniform_space;
		glgpu::Buffer uniform_object_color;
		glgpu::Buffer uniform_light;
		glgpu::Buffer uniform_camera;
		glgpu::Buffer uniform_material;

		//PBR is based on solving the rendering equation integral, for our case solving the diffuse and specular integrations only (excluding the emission part)
		/* 1) DIFFUSE : we solve the diffuse part of the reflectance integral equation using irradiance convoluted cubemap (diffuse_irradiance_map)
			that corresponds to the contributions of all incoming light rays from the enviroment map
			through the hemipshere surrounding a given normal at a given point*/
		glgpu::Cubemap diffuse_irradiance_map;
		glgpu::Sampler sampler_diffuse;

		/* 2) SPECULAR : we solve the specular part of the reflectance integral equation using split sum approx algorithm by EPIC Games.
			Split sum approx algorithm splits the specular integral part of the reflectance equation to a simpler two integral parts :
			A) we solve the first part of the specular part of the reflectance integral equation using prefiltered convoluted map that
				corresponds to the contribution of the outcoming reflected rays at a given point of the surface contained by the specular lobe
				which changes according to the surface roughness (specular_prefiltered_map)
			B) we solve the second part of the integral using Lookup Texture of the normal-the incoming light ray angle and surface roughness,
				which represents the BRDF of the specular integral (specular_BRDF_LUT)
			Read till page 7 for more understanding : https://cdn2.unrealengine.com/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
		*/
		glgpu::Cubemap specular_prefiltered_map;
		glgpu::Sampler sampler_specular_prefiltering;

		glgpu::Texture specular_BRDF_LUT;
		glgpu::Sampler sampler_specular_BRDF;

		std::vector<const world::object3D*> meshes;
	};

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
		float dummy_padding[2];
	};

	PBR
	pbr_create()
	{
		IPBR* self = new IPBR;

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self->prog = program_create(DIR_PATH"/engine/shaders/pbr.vertex", DIR_PATH"/engine/shaders/pbr.pixel");
		self->uniform_space = buffer_uniform_create(sizeof(Space_Uniform));
		self->uniform_object_color = buffer_uniform_create(sizeof(vec4f));
		self->uniform_light = buffer_uniform_create(sizeof(Light_Uniform));
		self->uniform_camera = buffer_uniform_create(sizeof(Camera_Uniform));
		self->uniform_material = buffer_uniform_create(sizeof(Material_Uniform));
		self->sampler_diffuse = sampler_create(TEXTURE_FILTERING::LINEAR, TEXTURE_FILTERING::LINEAR, TEXTURE_SAMPLING::CLAMP_TO_EDGE);
		self->sampler_specular_prefiltering = sampler_create(TEXTURE_FILTERING::LINEAR_MIPMAP, TEXTURE_FILTERING::LINEAR, TEXTURE_SAMPLING::CLAMP_TO_EDGE);
		self->sampler_specular_BRDF = sampler_create(TEXTURE_FILTERING::NEAREST, TEXTURE_FILTERING::NEAREST, TEXTURE_SAMPLING::REPEAT);

		/*Diffuse irriadiance convoluted map*/
		io::Image diff = image_read(DIR_PATH"/res/imgs/hdr/Tokyo_diff.hdr", io::IMAGE_FORMAT::HDR);
		self->diffuse_irradiance_map = cubemap_hdr_create(diff, vec2f{512, 512}, false);
		image_free(diff);

		//Specular prefiltered convoluted map (Part 1 from the specular integration of the reflectance equation)
		//1) create prefiltered convolution map with mipmap generated, rougher = blurrier reflections
		//2) create env_cmap that will be used for convolution to create prefilterd map
		//3) convolute env_cmap 5 times (mipmaps) using prefiltering shader and save them to specular_prefiltered_map mipmaps, diff roughness = diff prefiltered map
		vec2f prefiltered_initial_size{ 128, 128 };
		self->specular_prefiltered_map = cubemap_create(prefiltered_initial_size, INTERNAL_TEXTURE_FORMAT::RGB16F, EXTERNAL_TEXTURE_FORMAT::RGB, DATA_TYPE::FLOAT, true);

		io::Image env = image_read(DIR_PATH"/res/imgs/hdr/Tokyo_spec.hdr", io::IMAGE_FORMAT::HDR);
		Cubemap env_cmap = cubemap_hdr_create(env, vec2f{ 512, 512 }, true);

		Program prefiltering_prog = program_create(DIR_PATH"/engine/shaders/cube.vertex", DIR_PATH"/engine/shaders/specular_prefiltering_convolution.pixel");
		unsigned int max_mipmaps = 5;
		for (unsigned int mip_level = 0; mip_level < max_mipmaps; ++mip_level)
		{
			float roughness = (float)mip_level / max_mipmaps;
			vec2f mipmap_size{ prefiltered_initial_size[0] * std::pow(0.5, mip_level) , prefiltered_initial_size[0] * std::pow(0.5, mip_level) };
			cubemap_postprocess(env_cmap, self->specular_prefiltered_map, prefiltering_prog, Unifrom_Float{"roughness", roughness}, mipmap_size, mip_level);
		}
		program_delete(prefiltering_prog);
		cubemap_free(env_cmap);
		image_free(env);

		//Specular BRDF convoluted LUT (Part 2 from the specular integration of the reflectance equation)
		Program BRDF_prog = program_create(DIR_PATH"/engine/shaders/quad_ndc.vertex", DIR_PATH"/engine/shaders/specular_BRDF_convolution.pixel");
		vec2f BRDF_LUT_size{ 512, 512 };
		self->specular_BRDF_LUT = texture2d_create(BRDF_LUT_size, INTERNAL_TEXTURE_FORMAT::RG16F, EXTERNAL_TEXTURE_FORMAT::RG, DATA_TYPE::FLOAT, false);
		texture2d_render_offline_to(self->specular_BRDF_LUT, BRDF_prog, BRDF_LUT_size);
		program_delete(BRDF_prog);

		return self;
	}

	void
	pbr_free(PBR self)
	{
		program_delete(self->prog);
		buffer_delete(self->uniform_space);
		buffer_delete(self->uniform_object_color);
		buffer_delete(self->uniform_light);
		buffer_delete(self->uniform_camera);
		buffer_delete(self->uniform_material);
		cubemap_free(self->diffuse_irradiance_map);
		cubemap_free(self->specular_prefiltered_map);
		sampler_free(self->sampler_diffuse);
		sampler_free(self->sampler_specular_prefiltering);
		sampler_free(self->sampler_specular_BRDF);

		delete self;
	}
	
	void
	pbr_pack(PBR self, const world::object3D* mesh)
	{
		self->meshes.push_back(mesh);
	}

	void
	pbr_unpack(PBR self)
	{
		self->meshes.clear();
	}

	void
	pbr_draw(const PBR self, const Camera& camera)
	{
		color_clear(0.1f, 0.1f, 0.1f);
		program_use(self->prog);

		//viewport
		vec2f viewport = world::camera_viewport(camera);
		view_port(0, 0, (int)viewport[0], (int)viewport[1]);

		//uniform blocks
		buffer_uniform_bind(0, self->uniform_space);
		buffer_uniform_bind(1, self->uniform_object_color);
		buffer_uniform_bind(2, self->uniform_light);
		buffer_uniform_bind(3, self->uniform_camera);
		buffer_uniform_bind(4, self->uniform_material);

		//till we get sampler objects in
		cubemap_bind(self->diffuse_irradiance_map, 0);
		sampler_bind(self->sampler_diffuse, 0);
		cubemap_bind(self->specular_prefiltered_map, 1);
		sampler_bind(self->sampler_specular_prefiltering, 1);
		texture2d_bind(self->specular_BRDF_LUT, 2);
		sampler_bind(self->sampler_specular_BRDF, 2);

		for (auto object : self->meshes)
		{
			//uniform blocks
			Space_Uniform mvp{ mat4_from_transform(object->model), camera_view_proj(camera) };
			buffer_uniform_set(self->uniform_space, &mvp, sizeof(mvp));
			vec4f color_test{ 0.75f, 0.75f, 0.75f, 1.0f };
			buffer_uniform_set(self->uniform_object_color, &color_test, sizeof(color_test));
			Light_Uniform light{ vec4f{ 1.0f, 1.0f, 1.0f,1.0f }, vec4f{ 0.0f, -1.0f, 0.0f, 0.0f } };
			buffer_uniform_set(self->uniform_light, &light, sizeof(light));
			Camera_Uniform cam{ camera.pos[0], camera.pos[1], camera.pos[2], 0.0f };
			buffer_uniform_set(self->uniform_camera, &cam, sizeof(cam));
			Material_Uniform mat{ 0.9, 0.2, {} };
			buffer_uniform_set(self->uniform_material, &mat, sizeof(mat));

			//draw geometry
			vao_bind(object->mesh.va);
			draw_indexed(object->mesh.indices.size());
			vao_unbind();
		}
	}
}