#include "engine/renderer/Point.h"

#include "gl/glgpu.h"

#include "defs/Defs.h"

#include <vector>

using namespace glgpu;

namespace rndr
{
	struct IPoint
	{
		glgpu::Program prog;
		glgpu::Buffer uniform_space;
		glgpu::Buffer uniform_color;
		glgpu::Buffer vbo;
		glgpu::Vao vao;
		std::vector<math::vec3f> points;
	};

	//Careful of std140 memory layout/alignment/offsets.prefer always to allocate chunks of 16 bytes.
	//Always check renderdoc.
	struct Space_Uniform
	{
		math::Mat4f mvp;
	};

	struct Color_Uniform
	{
		math::vec4f col;
	};

	Point
	point_create()
	{
		IPoint* self = new IPoint;
		self->prog = program_create(DIR_PATH"/engine/shaders/point.vertex", DIR_PATH"/engine/shaders/point.pixel");
		self->vbo = buffer_vertex_create();
		self->vao = vao_create();
		vao_attach(self->vao, self->vbo);
		buffer_vertex_attribute(self->vbo, 0, 3, sizeof(math::vec3f), 0);
		vao_unbind();

		self->uniform_space = buffer_uniform_create(sizeof(Space_Uniform));
		self->uniform_color = buffer_uniform_create(sizeof(Color_Uniform));
		return self;
	}

	void
	point_free(Point self)
	{
		program_delete(self->prog);
		buffer_delete(self->vbo);
		buffer_delete(self->uniform_space);
		buffer_delete(self->uniform_color);
		delete self;
	}

	void
	point_append(Point self, math::vec3f& point)
	{
		self->points.emplace_back(point);
	}

	void
	point_set(Point self, const math::Mat4f& view_proj, math::vec4f& col)
	{
		program_use(self->prog);

		buffer_vertex_set(self->vbo, &self->points.front(), self->points.size(), Storage::DYNAMIC);
		buffer_uniform_bind(0, self->uniform_space);
		buffer_uniform_bind(1, self->uniform_color);

		Space_Uniform mvp{ view_proj };
		buffer_uniform_set(self->uniform_space, &mvp, sizeof(mvp));
		buffer_uniform_set(self->uniform_color, (void*)&col, sizeof(col));
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
