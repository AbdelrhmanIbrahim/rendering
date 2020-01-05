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

		//irriadiance
		io::Image diff = image_read("../rendering/res/imgs/hdr/Tokyo_diff.hdr", io::IMAGE_FORMAT::HDR);
		self.diffuse_irradiance_map = cubemap_hdr_create(diff, vec2f{512, 512});
		image_free(diff);

		//specular prefiltered convoluted map (Part 1 from the specular integration of the reflectance equation)
		//1) create specular env cubemap
		io::Image env = image_read("../rendering/res/imgs/hdr/Tokyo_spec.hdr", io::IMAGE_FORMAT::HDR);
		cubemap env_cmap = cubemap_hdr_create(env, vec2f{512, 512});

		//2) create prefiltered convolution map with mipmap generated 
		//(for different roughness -> different reflective specular properties, rougher = blurrier reflections)
		vec2f size{ 128, 128 };
		self.specular_prefiltered_map = cubemap_create(size, INTERNAL_TEXTURE_FORMAT::RGB16F, EXTERNAL_TEXTURE_FORMAT::RGB, DATA_TYPE::FLOAT);
		mipmap_generate(TARGET::CUBEMAP);

		//3) convolute env_cmap 5 times using prefiltering shader (mipmap) and save them to specular_prefiltered_map mipmaps
		for (unsigned int mip_level = 0; mip_level < 5; ++mip_level)
		{
			unsigned int mipw = size[0] * std::pow(0.5, mip_level);
			unsigned int miph = size[1] * std::pow(0.5, mip_level);
			vec2f mipmap_size{ mipw , miph};
			//convolute and attach the convoluted cubemap to each level of mip and roughness
			//cubemap_postprocess(env_cmap, self.specular_prefiltered_map, mipmap_size, mip_level, "../rendering/engine/shaders/specular_prefiltering_convolution.pixel", uniforms);
		}

		cubemap_free(env_cmap);
		image_free(env);

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
		uniform1i_set(self.prog, "irradiance_map", TEXTURE_UNIT::UNIT_0);

		for (const auto object : self.meshes)
		{
			//MVP
			Mat4f model = mat4_from_transform(object->model);
			Mat4f mvp = camera_view_proj(cam) * model;

			uniformmat4f_set(self.prog, "mvp", mvp);
			uniformmat4f_set(self.prog, "model", model);
			uniform3f_set(self.prog, "object_color_albedo", vec3f{ 0.75f, 0.75f, 0.75f});
			uniform1f_set(self.prog, "metallic", 0.7);
			uniform1f_set(self.prog, "rough", 0.4);

			//draw geometry
			vao_bind(object->mesh.va, object->mesh.vs, object->mesh.is);
			draw_indexed(object->mesh.indices.size());
			vao_unbind();
		}
	}
}