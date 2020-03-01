#include "Phong.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

#include "GL/glgpu.h"

#include <vector>

using namespace glgpu;
using namespace math;
using namespace world;

namespace rndr
{
	struct IPhong
	{
		glgpu::Program prog;
		glgpu::Buffer uniform_object_color;
		glgpu::Buffer uniform_space;
		glgpu::Buffer uniform_light;
		glgpu::Buffer uniform_camera;
		std::vector<const world::object3D*> meshes;
	};

	//Careful of std140 memory layout/alignment/offsets.prefer always to allocate chunks of 16 bytes.
	//Always check renderdoc.
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

	Phong
	phong_create()
	{
		IPhong* self = new IPhong();

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self->prog = program_create(DIR_PATH"/engine/shaders/phong.vertex", DIR_PATH"/engine/shaders/phong.pixel");
		self->uniform_object_color = buffer_uniform_create(sizeof(vec4f));
		self->uniform_space = buffer_uniform_create(sizeof(Space_Uniform));
		self->uniform_light = buffer_uniform_create(sizeof(Light_Uniform));
		self->uniform_camera = buffer_uniform_create(sizeof(Camera_Uniform));
		return self;
	}

	void
	phong_free(Phong self)
	{
		program_delete(self->prog);
		buffer_delete(self->uniform_object_color);
		buffer_delete(self->uniform_space);
		buffer_delete(self->uniform_light);
		buffer_delete(self->uniform_camera);

		delete self;
	}

	void
	phong_pack(Phong self, const world::object3D* mesh)
	{
		self->meshes.push_back(mesh);
	}

	void
	phong_unpack(Phong self)
	{
		self->meshes.clear();
	}

	void
	phong_draw(const Phong self, const Camera& camera)
	{
		color_clear(0.1f, 0.1f, 0.1f);
		program_use(self->prog);

		//viewport
		vec2f viewport = world::camera_viewport(camera);
		view_port(0, 0, (int)viewport[0], (int)viewport[1]);

		//uniform block
		buffer_uniform_bind(0, self->uniform_space);
		buffer_uniform_bind(1, self->uniform_object_color);
		buffer_uniform_bind(2, self->uniform_light);
		buffer_uniform_bind(3, self->uniform_camera);

		for (auto object : self->meshes)
		{
			//uniform blocks
			Space_Uniform mvp{ mat4_from_transform(object->model), camera_view_proj(camera) };
			buffer_uniform_set(self->uniform_space, &mvp, sizeof(mvp));
			vec4f color_test{ 0.0, 0.5, 0.31, 1.0f };
			buffer_uniform_set(self->uniform_object_color, &color_test, sizeof(color_test));
			Light_Uniform light{ vec4f{ 1.0f, 1.0f, 1.0f,1.0f }, vec4f{ 0.0f, -1.0f, 0.0f, 0.0f } };
			buffer_uniform_set(self->uniform_light, &light, sizeof(light));
			Camera_Uniform cam{ camera.pos[0], camera.pos[1], camera.pos[2], 0.0f };
			buffer_uniform_set(self->uniform_camera, &cam, sizeof(cam));

			//draw geometry
			vao_bind(object->mesh.va);
			draw_indexed(object->mesh.indices.size());
			vao_unbind();
		}
	}
}