#include "engine/renderer/Quad.h"

#include "gl/glgpu.h"

using namespace glgpu;

namespace rndr
{
	struct IQuad
	{
		Program prog;
		Vao quad_vao;
		Buffer quad_vbo;
		Buffer uvp;
	};

	Quad
	quad_create()
	{
		IQuad* self = new IQuad;

		self->prog = program_create(DIR_PATH"/engine/shaders/cquad.vertex", DIR_PATH"/engine/shaders/cquad.pixel");
		self->quad_vao = vao_create();
		self->quad_vbo = buffer_vertex_create();
		self->uvp = buffer_uniform_create(sizeof(math::Mat4f));

		return self;
	}

	void
	quad_free(Quad self)
	{
		program_delete(self->prog);
		vao_delete(self->quad_vao);
		buffer_delete(self->quad_vbo);
		buffer_delete(self->uvp);
	}

	void
	quad_set(Quad self, math::Mat4f& view_proj, math::vec2f& viewport)
	{
		program_use(self->prog);
		buffer_uniform_bind(0, self->uvp);
		buffer_uniform_set(self->uvp, &view_proj, sizeof(view_proj));
		view_port(0, 0, (int)viewport[0], (int)viewport[1]);
	}

	void
	quad_draw(Quad self, const world::CVertex quad_strip[6])
	{
		vao_attach(self->quad_vao, self->quad_vbo);
		buffer_vertex_set(self->quad_vbo, quad_strip, sizeof(quad_strip), STORAGE::DYNAMIC);
		buffer_vertex_attribute(self->quad_vbo, 0, 3, sizeof(world::CVertex), 0);
		buffer_vertex_attribute(self->quad_vbo, 1, 4, sizeof(world::CVertex), sizeof(world::CVertex::pos));

		//draw geometry
		vao_bind(self->quad_vao);
		draw_strips(6);
		vao_unbind();
	}
};