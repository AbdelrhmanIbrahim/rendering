#include "Mesh_Renderer.h"

#include "IO/Image.h"

using namespace glgpu;
using namespace io;

namespace rndr
{
	Mesh_Renderer
	mesh_renderer_create()
	{
		Mesh_Renderer self{};

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self.prog = program_create("../rendering/engine/shaders/texture.vertex", "../rendering/engine/shaders/texture.pixel");
		self.tex = texture_create("../rendering/res/images/container.jpg");

		return self;
	}

	void
	mesh_renderer_free(const Mesh_Renderer & mr)
	{
		program_delete(mr.prog);
		texture_free(mr.tex);
	}

	void
	mesh_renderer_draw(const Mesh_Renderer& mr, const geo::Mesh& mesh, math::vec4f& color)
	{
		color_clear(0.0f, 1.0f, 0.0f);
		program_use(mr.prog);

		//texture settings
		texture_bind(mr.tex, TEXTURE_UNIT::UNIT_0);
		uniform1i_set(mr.prog, "texture_0", TEXTURE_UNIT::UNIT_0);

		//draw geometry
		vao_bind(mesh.va, mesh.vs, mesh.is);
		draw_indexed(sizeof(mesh.indices));
		vao_unbind();
	}
}