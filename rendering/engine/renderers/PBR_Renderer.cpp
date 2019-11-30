#include "PBR_Renderer.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

using namespace glgpu;
using namespace math;
using namespace world;

namespace rndr
{
	PBR_Renderer
	pbr_renderer_create()
	{
		PBR_Renderer self{};

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self.prog = program_create("../rendering/engine/shaders/pbr.vertex", "../rendering/engine/shaders/pbr.pixel");

		return self;
	}

	void
	pbr_renderer_free(PBR_Renderer & mr)
	{
		program_delete(mr.prog);
	}

	void
	pbr_renderer_draw(const PBR_Renderer& mr, const world::object3D& object, const Camera& cam)
	{
		color_clear(0.1f, 0.1f, 0.1f);
		program_use(mr.prog);

		//MVP
		Mat4f model = mat4_from_transform(object.model);
		Mat4f mvp = camera_view_proj(cam) * model;

		uniformmat4f_set(mr.prog, "mvp", mvp);
		uniformmat4f_set(mr.prog, "model", model);
		uniform3f_set(mr.prog, "camera_pos_world", cam.pos);
		uniform3f_set(mr.prog, "light_color", vec3f{ 1.0f, 1.0f, 1.0f });
		uniform3f_set(mr.prog, "light_pos_world", model * vec4f{ 3.0f, 3.0f, 0.0f, 1.0f });
		uniform3f_set(mr.prog, "object_color_albedo", vec3f{ 1.0, 0.5, 0.31 });
		uniform1f_set(mr.prog, "metallic", 1.0);
		uniform1f_set(mr.prog, "rough", 0.5);

		//viewport
		vec2f viewport = world::camera_viewport(cam);
		view_port(0, 0, (int)viewport[0], (int)viewport[1]);

		//draw geometry
		vao_bind(object.mesh.va, object.mesh.vs, object.mesh.is);
		draw_indexed(object.mesh.indices.size());
		vao_unbind();
	}
}