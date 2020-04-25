#include "Phong.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

#include "gl/glgpu.h"

#include "defs/Defs.h"

#include "world/components/Camera.h"
#include "world/components/Mesh.h"
#include "world/components/Transform.h"
#include "world/components/Material.h"
#include "world/components/Sun.h"
#include "world/components/Lamp.h"
#include "world/components/Flash.h"

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

	constexpr int MAX_NUMBER_LIGHT = 10;
	struct
	Sun_Uniform
	{
		int count;
		int dummy[3];
		world::Sun arr[MAX_NUMBER_LIGHT];
	};

	struct
	Lamp_Uniform
	{
		int count;
		int dummy[3];
		world::Lamp arr[MAX_NUMBER_LIGHT];
	};

	struct
	Flash_Uniform
	{
		int count;
		int dummy[3];
		world::Flash arr[MAX_NUMBER_LIGHT];
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
	phong_init(Phong self, math::vec2f viewport)
	{
		color_clear(0.1f, 0.1f, 0.1f);
		program_use(self->prog);

		buffer_uniform_bind(0, self->uniform_space);
		buffer_uniform_bind(1, self->uniform_object_color);
		buffer_uniform_bind(2, self->uniform_camera);
		buffer_uniform_bind(3, self->uniform_suns);
		buffer_uniform_bind(4, self->uniform_lamps);
		buffer_uniform_bind(5, self->uniform_flashs);

		view_port(0, 0, (int)viewport[0], (int)viewport[1]);
	}

	void
	phong_draw(const Phong self, const world::Camera* camera, const world::Mesh* mesh, const world::Transform* model, const world::Material* material)
	{
		Space_Uniform mvp{mat4_from_transform(*model), camera_view_proj(*camera) };
		buffer_uniform_set(self->uniform_space, &mvp, sizeof(mvp));
		buffer_uniform_set(self->uniform_object_color, (void*)&material->color_norm, sizeof(material->color_norm));

		Camera_Uniform cam{ camera->pos[0], camera->pos[1], camera->pos[2], 0.0f };
		buffer_uniform_set(self->uniform_camera, &cam, sizeof(cam));

		Sun_Uniform suns
		{
			0, {}, 
			{
				world::Sun {{1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, -1.0f, 0.0f, 0.0f }},
			}
		};
		buffer_uniform_set(self->uniform_suns, &suns, sizeof(suns));

		Lamp_Uniform lamps
		{
			1, {},
			{
				world::Lamp {{1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f, 0.0f, 0.0f }, 10},
			}
		};
		buffer_uniform_set(self->uniform_lamps, &lamps, sizeof(lamps));

		Flash_Uniform flashes
		{
			0, {},
			{
				world::Flash {{1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f, 0.0f, 0.0f }, {0, 0, -1, 0}, 10, (float)cos(to_radian(12)), (float)cos(to_radian(15))},
			}
		};
		buffer_uniform_set(self->uniform_flashs, &flashes, sizeof(flashes));

		//draw geometry
		vao_bind(mesh->va);
		draw_indexed(mesh->indices.size());
		vao_unbind();
	}
}