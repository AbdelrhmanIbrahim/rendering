#include "engine/renderer/Point.h"

#include "gl/glgpu.h"

#include "math/Vector.h"

#include "defs/Defs.h"

#include <vector>

using namespace glgpu;

namespace rndr
{
	struct
	Pnt
	{
		math::vec3f pos;
		math::vec4f col;
	};

	struct IPoint
	{
		glgpu::Program prog;
		glgpu::Buffer uniform_space;
		glgpu::Buffer vbo;
		glgpu::Vao vao;
		std::vector<Pnt> points;
	};

	//Careful of std140 memory layout/alignment/offsets.prefer always to allocate chunks of 16 bytes.
	//Always check renderdoc.
	struct Space_Uniform
	{
		math::Mat4f mvp;
	};

	Point
	point_create()
	{
		IPoint* self = new IPoint;
		self->prog = program_create(DIR_PATH"/engine/shaders/point.vertex", DIR_PATH"/engine/shaders/point.pixel");
		self->uniform_space = buffer_uniform_create(sizeof(Space_Uniform));
		self->vbo = buffer_vertex_create();
		self->vao = vao_create();
		vao_attach(self->vao, self->vbo);
		buffer_vertex_attribute(self->vbo, 0, 3, sizeof(Pnt), 0);
		buffer_vertex_attribute(self->vbo, 1, 4, sizeof(Pnt), 3 * sizeof(float));
		vao_unbind();

		return self;
	}

	void
	point_free(Point self)
	{
		program_delete(self->prog);
		buffer_delete(self->vbo);
		buffer_delete(self->uniform_space);
		delete self;
	}

	void
	point_append(Point self, const math::vec3f& pos, const math::vec4f& col)
	{
		self->points.emplace_back(Pnt{pos, col});
	}

	void
	point_set(Point self, const math::Mat4f& view_proj)
	{
		program_use(self->prog);

		Space_Uniform mvp{ view_proj };
		buffer_uniform_set(self->uniform_space, &mvp, sizeof(mvp));
		buffer_vertex_set(self->vbo, &self->points.front(), self->points.size() * sizeof(self->points.front()) , Storage::DYNAMIC);
		buffer_uniform_bind(0, self->uniform_space);
	}

	void
	point_draw(Point self)
	{
		vao_bind(self->vao);
		draw_point(self->points.size());
		vao_unbind();
		self->points.clear();
	}
};
