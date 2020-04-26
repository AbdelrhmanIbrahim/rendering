#include "Phong.h"

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
		glgpu::Buffer uniform_suns;
		glgpu::Buffer uniform_lamps;
		glgpu::Buffer uniform_flashs;
		glgpu::Buffer uniform_camera;
	};

	//Careful of std140 memory layout/alignment/offsets.prefer always to allocate chunks of 16 bytes.
	//Always check renderdoc.
	struct Space_Uniform
	{
		Mat4f model;
		Mat4f vp;
	};

	struct Camera_Uniform
	{
		vec4f world_pos;
	};

	
	struct
	Sun_Uniform
	{
		int count;
		int dummy[3];
		world::Sun arr[MAX_NUMBER_LIGHT_TYPE];
	};

	struct
	Lamp_Uniform
	{
		int count;
		int dummy[3];
		world::Lamp arr[MAX_NUMBER_LIGHT_TYPE];
	};

	struct
	Flash_Uniform
	{
		int count;
		int dummy[3];
		world::Flash arr[MAX_NUMBER_LIGHT_TYPE];
	};

	Phong
	phong_create()
	{
		IPhong* self = new IPhong();

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self->prog = program_create(DIR_PATH"/engine/shaders/phong.vertex", DIR_PATH"/engine/shaders/phong.pixel");
		self->uniform_object_color = buffer_uniform_create(sizeof(vec4f));
		self->uniform_space = buffer_uniform_create(sizeof(Space_Uniform));
		self->uniform_suns = buffer_uniform_create(sizeof(Sun_Uniform));
		self->uniform_lamps = buffer_uniform_create(sizeof(Lamp_Uniform));
		self->uniform_flashs = buffer_uniform_create(sizeof(Flash_Uniform));
		self->uniform_camera = buffer_uniform_create(sizeof(Camera_Uniform));
		return self;
	}

	void
	phong_free(Phong self)
	{
		program_delete(self->prog);
		buffer_delete(self->uniform_object_color);
		buffer_delete(self->uniform_space);
		buffer_delete(self->uniform_suns);
		buffer_delete(self->uniform_lamps);
		buffer_delete(self->uniform_flashs);
		buffer_delete(self->uniform_camera);

		delete self;
	}

	void
	phong_set(Phong self, 
		const world::Camera* camera, 
		const std::vector<world::Sun>& suns,
		const std::vector<world::Lamp>& lamps,
		const std::vector<world::Flash>& flashes)
	{
		color_clear(0.1f, 0.1f, 0.1f);
		program_use(self->prog);

		buffer_uniform_bind(0, self->uniform_space);
		buffer_uniform_bind(1, self->uniform_object_color);
		buffer_uniform_bind(2, self->uniform_camera);
		buffer_uniform_bind(3, self->uniform_suns);
		buffer_uniform_bind(4, self->uniform_lamps);
		buffer_uniform_bind(5, self->uniform_flashs);

		Camera_Uniform cam{ camera->pos[0], camera->pos[1], camera->pos[2], 0.0f };
		buffer_uniform_set(self->uniform_camera, &cam, sizeof(cam));

		math::vec2f viewport = camera_viewport(*camera);
		view_port(0, 0, (int)viewport[0], (int)viewport[1]);

		//unnecassery frequent uniforms with arrays creation -revisit-
		Sun_Uniform suns_u{suns.size() % MAX_NUMBER_LIGHT_TYPE, {}};
		for (int i = 0; i < suns.size(); ++i)
			suns_u.arr[i%MAX_NUMBER_LIGHT_TYPE] = suns[i];
		buffer_uniform_set(self->uniform_suns, &suns_u, sizeof(suns_u));

		Lamp_Uniform lamps_u {lamps.size() % MAX_NUMBER_LIGHT_TYPE, {}};
		for (int i = 0; i < lamps.size(); ++i)
			lamps_u.arr[i % MAX_NUMBER_LIGHT_TYPE] = lamps[i];
		buffer_uniform_set(self->uniform_lamps, &lamps_u, sizeof(lamps_u));

		Flash_Uniform flashes_u {flashes.size() % MAX_NUMBER_LIGHT_TYPE, {}};
		for (int i = 0; i < flashes.size(); ++i)
			flashes_u.arr[i % MAX_NUMBER_LIGHT_TYPE] = flashes[i];
		buffer_uniform_set(self->uniform_flashs, &flashes_u, sizeof(flashes_u));
	}

	void
	phong_draw(const Phong self, 
		const math::Mat4f& view_proj, 
		const world::Mesh* mesh, 
		const world::Transform* model, 
		const world::Material* material)
	{
		Space_Uniform mvp{mat4_from_transform(*model), view_proj };
		buffer_uniform_set(self->uniform_space, &mvp, sizeof(mvp));
		buffer_uniform_set(self->uniform_object_color, (void*)&material->color_norm, sizeof(material->color_norm));

		vao_bind(mesh->va);
		draw_indexed(mesh->indices.size());
		vao_unbind();
	}
}