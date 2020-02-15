#include "Phong_Renderer.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

using namespace glgpu;
using namespace math;
using namespace world;

namespace rndr
{
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

	Phong_Renderer
	phong_create()
	{
		Phong_Renderer self{};

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self.prog = program_create("F:/Abdo/rendering_jo/rendering/engine/shaders/phong.vertex", "F:/Abdo/rendering_jo/rendering/engine/shaders/phong.pixel");
		self.uniform_object_color = buffer_uniform_create(sizeof(vec4f));
		self.uniform_space = buffer_uniform_create(sizeof(Space_Uniform));
		self.uniform_light = buffer_uniform_create(sizeof(Light_Uniform));
		self.uniform_camera = buffer_uniform_create(sizeof(Camera_Uniform));
		return self;
	}

	void
	phong_free(Phong_Renderer & mr)
	{
		program_delete(mr.prog);
		buffer_delete(mr.uniform_object_color);
		buffer_delete(mr.uniform_space);
		buffer_delete(mr.uniform_light);
		buffer_delete(mr.uniform_camera);
	}

	void
	phong_pack(Phong_Renderer& mr, const world::object3D* mesh)
	{
		mr.meshes.push_back(mesh);
	}

	void
	phong_unpack(Phong_Renderer & mr)
	{
		mr.meshes.clear();
	}

	void
	phong_draw(const Phong_Renderer& mr, const Camera& camera)
	{
		color_clear(0.1f, 0.1f, 0.1f);
		program_use(mr.prog);

		//viewport
		vec2f viewport = world::camera_viewport(camera);
		view_port(0, 0, (int)viewport[0], (int)viewport[1]);

		//uniform block
		buffer_uniform_bind(0, mr.uniform_space);
		buffer_uniform_bind(1, mr.uniform_object_color);
		buffer_uniform_bind(2, mr.uniform_light);
		buffer_uniform_bind(3, mr.uniform_camera);

		for (auto object : mr.meshes)
		{
			//uniform blocks
			Space_Uniform mvp{ mat4_from_transform(object->model), camera_view_proj(camera) };
			buffer_uniform_set(mr.uniform_space, &mvp, sizeof(mvp));
			vec4f color_test{ 0.0, 0.5, 0.31, 1.0f };
			buffer_uniform_set(mr.uniform_object_color, &color_test, sizeof(color_test));
			Light_Uniform light{ vec4f{ 1.0f, 1.0f, 1.0f,1.0f }, vec4f{ 0.0f, -1.0f, 0.0f, 0.0f } };
			buffer_uniform_set(mr.uniform_light, &light, sizeof(light));
			Camera_Uniform cam{ camera.pos[0], camera.pos[1], camera.pos[2], 0.0f };
			buffer_uniform_set(mr.uniform_camera, &cam, sizeof(cam));

			//draw geometry
			vao_bind(object->mesh.va, object->mesh.vs, object->mesh.is);
			draw_indexed(object->mesh.indices.size());
			vao_unbind();
		}
	}
}