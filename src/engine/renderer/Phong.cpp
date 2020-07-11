#include "engine/renderer/Phong.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

#include "gl/glgpu.h"

#include "defs/Defs.h"

#include "world/component/Camera.h"
#include "world/component/Mesh.h"
#include "world/component/Transform.h"
#include "world/component/Material.h"

#include <corecrt_math.h>

using namespace glgpu;
using namespace math;

namespace rndr
{
	struct IPhong
	{
		glgpu::Program prog;
		glgpu::Buffer uniform_object_color;
		glgpu::Buffer uniform_space;
		glgpu::Buffer uniform_light_count;
		glgpu::Buffer uniform_suns;
		glgpu::Buffer uniform_lamps;
		glgpu::Buffer uniform_flashes;
		glgpu::Buffer uniform_camera;
	};

	//Careful of std140 memory layout/alignment/offsets.prefer always to allocate chunks of 16 bytes.
	//Always check renderdoc.
	struct
	Space_Uniform
	{
		Mat4f model;
		Mat4f vp;
		Mat4f nomral_mat;
	};

	struct
	Camera_Uniform
	{
		vec4f world_pos;
	};

	struct
	Lights_Count_Uniform
	{
		int suns_count;
		int lamps_count;
		int flashes_count;
		int dummy;
	};

	Phong
	phong_create()
	{
		IPhong* self = new IPhong();

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self->prog = program_create(DIR_PATH"/src/engine/shaders/phong.vertex", DIR_PATH"/src/engine/shaders/phong.pixel");
		self->uniform_object_color = buffer_uniform_create(sizeof(vec4f));
		self->uniform_space = buffer_uniform_create(sizeof(Space_Uniform));
		self->uniform_light_count = buffer_uniform_create(sizeof(Lights_Count_Uniform));
		self->uniform_suns = buffer_uniform_create(MAX_NUMBER_LIGHT_TYPE * sizeof(world::Sun));
		self->uniform_lamps = buffer_uniform_create(MAX_NUMBER_LIGHT_TYPE * sizeof(world::Lamp));
		self->uniform_flashes = buffer_uniform_create(MAX_NUMBER_LIGHT_TYPE * sizeof(world::Flash));
		self->uniform_camera = buffer_uniform_create(sizeof(Camera_Uniform));
		return self;
	}

	void
	phong_free(Phong self)
	{
		program_delete(self->prog);
		buffer_delete(self->uniform_object_color);
		buffer_delete(self->uniform_space);
		buffer_delete(self->uniform_light_count);
		buffer_delete(self->uniform_suns);
		buffer_delete(self->uniform_lamps);
		buffer_delete(self->uniform_flashes);
		buffer_delete(self->uniform_camera);

		delete self;
	}

	void
	phong_set(Phong self, 
		world::Camera* camera,
		infra::mem::chunk<world::Sun> suns,
		infra::mem::chunk<world::Lamp> lamps,
		infra::mem::chunk<world::Flash> flashes)
	{
		program_use(self->prog);

		buffer_uniform_bind(0, self->uniform_space);
		buffer_uniform_bind(1, self->uniform_object_color);
		buffer_uniform_bind(2, self->uniform_camera);
		buffer_uniform_bind(3, self->uniform_light_count);
		buffer_uniform_bind(4, self->uniform_suns);
		buffer_uniform_bind(5, self->uniform_lamps);
		buffer_uniform_bind(6, self->uniform_flashes);

		Camera_Uniform cam{ camera->pos[0], camera->pos[1], camera->pos[2], 0.0f };
		buffer_uniform_set(self->uniform_camera, &cam, sizeof(cam));

		math::vec2f viewport = camera_viewport(*camera);
		view_port(0, 0, (int)viewport[0], (int)viewport[1]);

		//lights setting
		Lights_Count_Uniform lights_count{suns.size % MAX_NUMBER_LIGHT_TYPE, lamps.size % MAX_NUMBER_LIGHT_TYPE, flashes.size % MAX_NUMBER_LIGHT_TYPE};
		buffer_uniform_set(self->uniform_light_count, &lights_count, sizeof(lights_count));
		if(suns.size > 0)
			buffer_uniform_set(self->uniform_suns, suns.ptr, suns.size * sizeof(suns[0]));
		if (lamps.size > 0)
			buffer_uniform_set(self->uniform_lamps, lamps.ptr, lamps.size * sizeof(lamps[0]));
		if (flashes.size > 0)
			buffer_uniform_set(self->uniform_flashes, flashes.ptr, flashes.size * sizeof(flashes[0]));
	}

	void
	phong_draw(const Phong self, 
		const math::Mat4f& view_proj, 
		const world::Mesh* mesh, 
		const world::Transform* model, 
		const world::Material* material)
	{
		Mat4f model_mat = mat4_from_transform(*model);
		Space_Uniform mvp{model_mat, view_proj, model_mat };

		//non uniform scaling
		if (model->scale != vec3f{ model->scale[0],model->scale[0], model->scale[0] })
			mvp.nomral_mat = mat4_transpose(mat4_inverse(model_mat));
			
		buffer_uniform_set(self->uniform_space, &mvp, sizeof(mvp));
		buffer_uniform_set(self->uniform_object_color, (void*)&material->color_norm, sizeof(material->color_norm));

		//draw geometry
		vao_bind(mesh->vao);
		draw_indexed(mesh->indices.size());
		vao_unbind();
	}
}