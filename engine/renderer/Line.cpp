#include "engine/renderer/Line.h"

#include "gl/glgpu.h"

#include <vector>

using namespace glgpu;

namespace rndr
{
	struct ILine
	{
		glgpu::Program prog;
		glgpu::Buffer uniform_space;
		glgpu::Buffer vbo;
		glgpu::Vao vao;
		std::vector<world::CVertex> lines;
	};

	//Careful of std140 memory layout/alignment/offsets.prefer always to allocate chunks of 16 bytes.
	//Always check renderdoc.
	struct Space_Uniform
	{
		math::Mat4f mvp;
	};

	Line
	line_create()
	{
		ILine* self = new ILine;
		self->prog = program_create(DIR_PATH"/engine/shaders/line.vertex", DIR_PATH"/engine/shaders/line.pixel");
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
	line_free(Line self)
	{
		program_delete(self->prog);
		buffer_delete(self->vbo);
		buffer_delete(self->uniform_space);
		delete self;
	}

	void
	line_append(Line self, world::CVertex p0, world::CVertex p1)
	{
		self->lines.emplace_back(p0);
		self->lines.emplace_back(p1);
	}

	void
	line_set(Line self, const math::Mat4f& view_proj)
	{
		program_use(self->prog);

		Space_Uniform mvp{ view_proj };
		buffer_uniform_set(self->uniform_space, &mvp, sizeof(mvp));
		buffer_vertex_set(self->vbo, &self->lines.front(), self->lines.size() * sizeof(self->lines.front()), STORAGE::DYNAMIC);
		buffer_uniform_bind(0, self->uniform_space);
	}

	void
	line_draw(Line self)
	{
		vao_bind(self->vao);
		draw_lines(self->lines.size(), 5);
		vao_unbind();
		self->lines.clear();
	}
};
