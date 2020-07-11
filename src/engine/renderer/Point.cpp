#include "engine/renderer/Point.h"

#include "gl/glgpu.h"

#include <vector>

using namespace glgpu;

namespace rndr
{
	struct IPoint
	{
		glgpu::Program prog;
		glgpu::Buffer uniform_space;
		glgpu::Buffer vbo;
		glgpu::Vao vao;
		std::vector<world::CVertex> points;
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
		self->prog = program_create(DIR_PATH"/src/engine/shaders/point.vertex", DIR_PATH"/src/engine/shaders/point.pixel");
		self->uniform_space = buffer_uniform_create(sizeof(Space_Uniform));
		self->vbo = buffer_vertex_create();
		self->vao = vao_create();
		vao_attach(self->vao, self->vbo);
		buffer_vertex_attribute(self->vbo, 0, 3, sizeof(world::CVertex), 0);
		buffer_vertex_attribute(self->vbo, 1, 4, sizeof(world::CVertex), 3 * sizeof(float));
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
	point_append(Point self, world::CVertex point)
	{
		self->points.emplace_back(point);
	}

	void
	point_set(Point self, const math::Mat4f& view_proj)
	{
		program_use(self->prog);

		Space_Uniform mvp{ view_proj };
		buffer_uniform_set(self->uniform_space, &mvp, sizeof(mvp));
		buffer_vertex_set(self->vbo, &self->points.front(), self->points.size() * sizeof(self->points.front()) , STORAGE::DYNAMIC);
		buffer_uniform_bind(self->uniform_space, 0);
	}

	void
	point_draw(Point self)
	{
		vao_bind(self->vao);
		draw_points(self->points.size(), 10);
		vao_unbind();
		self->points.clear();
	}
};
