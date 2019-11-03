#include "Mesh_Renderer.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

using namespace glgpu;
using namespace math;
using namespace world;

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
	mesh_renderer_draw(const Mesh_Renderer& mr, const world::object3D& object, const Camera& cam)
	{
		color_clear(0.0f, 1.0f, 0.0f);
		program_use(mr.prog);

		//texture
		texture_bind(mr.tex, TEXTURE_UNIT::UNIT_0);
		uniform1i_set(mr.prog, "texture_0", TEXTURE_UNIT::UNIT_0);

		//MVP
		vec4f po{ 1,1,1,1 };
		auto tets = mat4_from_transform(object.model) * po;
		auto testss = camera_view_proj(cam)  * tets;
		uniformmat4f_set(mr.prog, "mvp", camera_view_proj(cam) * mat4_from_transform(object.model));

		//viewport
		vec2f viewport = world::camera_viewport(cam);
		view_port(0, 0, viewport[0], viewport[1]);

		//draw geometry
		vao_bind(object.mesh.va, object.mesh.vs, object.mesh.is);
		draw_indexed(sizeof(object.mesh.indices));
		vao_unbind();
	}
}