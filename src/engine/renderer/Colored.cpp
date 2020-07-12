#include "engine/renderer/Colored.h"

#include "world/component/Mesh.h"
#include "world/component/Transform.h"

#include "math/Matrix.h"

#include "gl/glgpu.h"

using namespace math;
using namespace glgpu;

namespace rndr
{
	struct IColored
	{
		glgpu::Program prog;
		glgpu::Buffer uniform_space;
		glgpu::Buffer uniform_object_color;
	};

	//Careful of std140 memory layout/alignment/offsets.prefer always to allocate chunks of 16 bytes.
	//Always check renderdoc.
	struct Space_Uniform
	{
		Mat4f mvp;
	};

	Colored
	colored_create()
	{
		IColored *self = new IColored();

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self->prog = program_create(DIR_PATH"/src/engine/shaders/colored.vertex", DIR_PATH"/src/engine/shaders/colored.pixel");
		self->uniform_space = buffer_uniform_create(sizeof(Space_Uniform));
		self->uniform_object_color = buffer_uniform_create(sizeof(Vec4f));
		return self;
	}

	void
	colored_free(Colored self)
	{
		handle_free(self->prog);
		handle_free(self->uniform_object_color);
		handle_free(self->uniform_space);

		delete self;
	}

	void
	colored_set(Colored self, math::Vec2f viewport)
	{
		handle_bind(self->prog);
		buffer_uniform_bind(self->uniform_space, 0);
		buffer_uniform_bind(self->uniform_object_color, 1);
		view_port(0, 0, (int)viewport[0], (int)viewport[1]);
	}

	void
	colored_draw(const Colored self, const math::Mat4f& view_proj, const world::Mesh* mesh, const world::Transform* model, const math::Vec4f& col)
	{
		//uniform blocks
		Space_Uniform mvp{ view_proj * mat4_from_transform(*model) };
		buffer_uniform_set(self->uniform_space, &mvp, sizeof(mvp));
		buffer_uniform_set(self->uniform_object_color, (void*)&col, sizeof(col));

		//draw geometry
		handle_bind(mesh->vao);
		draw_indexed(mesh->indices.size());
		handle_unbind(glgpu::HANDLE_KIND::KIND_VAO);
	}
};