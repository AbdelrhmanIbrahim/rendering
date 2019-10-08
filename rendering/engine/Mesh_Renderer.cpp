#include "Mesh_Renderer.h"

using namespace glgpu;

namespace rndr
{
	Mesh_Renderer
	mesh_renderer_create()
	{
		Mesh_Renderer self{};
		//TODO, deploy shaders to bin when moving to cmake or create a src obj (revisit)
		self.prog = program_create("../rendering/engine/shaders/colored.vertex", "../rendering/engine/shaders/colored.pixel");
		return self;
	}

	void
	mesh_renderer_free(const Mesh_Renderer & mr)
	{
		program_delete(mr.prog);
	}

	void
	mesh_rendere_draw(const Mesh_Renderer& mr, const geo::Mesh& mesh)
	{
		glgpu::color_clear(0.0f, 1.0f, 0.0f);
		glgpu::program_use(mr.prog);
		mesh_draw(mesh);
	}
}