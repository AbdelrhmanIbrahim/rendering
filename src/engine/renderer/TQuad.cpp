#include "engine/renderer/TQuad.h"

using namespace glgpu;

namespace rndr
{
	struct ITQuad
	{
		Program prog;
		Vao quad_vao;
		Buffer quad_vbo;
		Buffer uvp;
		Sampler sampler;
		Texture bg;
	};

	TQuad
	tquad_create(const char* image_path, IMAGE_FORMAT format)
	{
		ITQuad* self = new ITQuad;

		self->prog = program_create(DIR_PATH"/src/engine/shaders/tquad.vertex", DIR_PATH"/src/engine/shaders/tquad.pixel");
		self->quad_vao = vao_create();
		self->quad_vbo = buffer_vertex_create();
		self->uvp = buffer_uniform_create(sizeof(math::Mat4f));
		self->sampler = sampler_create(TEXTURE_FILTERING::LINEAR, TEXTURE_FILTERING::LINEAR, TEXTURE_SAMPLING::CLAMP_TO_EDGE);
		self->bg = texture2d_create(image_path, format);

		vao_attach(self->quad_vao, self->quad_vbo);
		buffer_vertex_attribute(self->quad_vbo, 0, 3, sizeof(world::TVertex), 0);
		buffer_vertex_attribute(self->quad_vbo, 1, 3, sizeof(world::TVertex), sizeof(world::TVertex::pos));
		buffer_vertex_attribute(self->quad_vbo, 2, 2, sizeof(world::TVertex), sizeof(world::TVertex::pos) + sizeof(world::TVertex::normal));

		return self;
	}

	void
	tquad_free(TQuad self)
	{
		handle_free(self->prog);
		handle_free(self->quad_vao);
		handle_free(self->quad_vbo);
		handle_free(self->uvp);
		handle_free(self->sampler);
		handle_free(self->bg);
	}

	void
	tquad_set(TQuad self, math::Mat4f& view_proj, math::Vec2f& viewport)
	{
		program_use(self->prog);
		buffer_uniform_bind(self->uvp, 0);
		buffer_uniform_set(self->uvp, &view_proj, sizeof(view_proj));
		sampler_bind(self->sampler, 1);
		texture2d_bind(self->bg, 1);
		view_port(0, 0, (int)viewport[0], (int)viewport[1]);
	}

	void
	tquad_draw(TQuad self, const world::TVertex quad_strip[6])
	{
		//draw geometry
		buffer_vertex_set(self->quad_vbo, quad_strip, 6 * sizeof(world::TVertex), STORAGE::DYNAMIC);
		vao_bind(self->quad_vao);
		draw_strips(6);
		vao_unbind();
	}
};