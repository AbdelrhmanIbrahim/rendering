#include "Mesh_Renderer.h"

using namespace glgpu;
using namespace geo;

namespace rndr
{
	Mesh_Renderer
	mesh_renderer_create()
	{
		Mesh_Renderer self{};

		static char vs[] = "#version 330 core \n layout(location = 0) in vec3 aPos; \n void main(){gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}";
		static char ps[] = "#version 330 core \n out vec4 FragColor; \n void main(){FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);}";
		self.prog = program_create(vs, ps);
		
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