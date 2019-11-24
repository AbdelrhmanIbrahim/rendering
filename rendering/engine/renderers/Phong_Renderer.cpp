#include "Phong_Renderer.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

using namespace glgpu;
using namespace math;
using namespace world;

namespace rndr
{
	Phong_Renderer
	phong_renderer_create()
	{
		Phong_Renderer self{};

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self.prog = program_create("../rendering/engine/shaders/phong.vertex", "../rendering/engine/shaders/phong.pixel");
		self.tex = texture_create("../rendering/res/images/container.jpg");

		return self;
	}

	void
	phong_renderer_free(const Phong_Renderer & mr)
	{
		program_delete(mr.prog);
		texture_free(mr.tex);
	}

	void
	phong_renderer_draw(const Phong_Renderer& mr, const world::object3D& object, const Camera& cam)
	{
		color_clear(0.1f, 0.1f, 0.1f);
		program_use(mr.prog);

		//texture
		texture_bind(mr.tex, TEXTURE_UNIT::UNIT_0);
		uniform1i_set(mr.prog, "texture", TEXTURE_UNIT::UNIT_0);

		//MVP
		Mat4f model = mat4_from_transform(object.model);
		Mat4f mvp = camera_view_proj(cam) * model;
		vec3f light_pos_world = model * vec4f{ 1.0f, 1.0f, 0.0f, 1.0f };
		vec3f cam_pos = cam.pos;

		uniformmat4f_set(mr.prog, "mvp", mvp);
		uniformmat4f_set(mr.prog, "model", model);
		uniform3f_set(mr.prog, "object_color", vec3f{ 1.0, 0.5, 0.31});
		uniform3f_set(mr.prog, "light_color", vec3f{ 1.0f, 1.0f, 1.0f });
		uniform3f_set(mr.prog, "light_world_pos", light_pos_world);
		uniform3f_set(mr.prog, "camera_world_pos", cam_pos);

		//viewport
		vec2f viewport = world::camera_viewport(cam);
		view_port(0, 0, (int)viewport[0], (int)viewport[1]);

		//draw geometry
		vao_bind(object.mesh.va, object.mesh.vs, object.mesh.is);
		draw_strip(object.mesh.vertices.size());
		//draw_indexed(object.mesh.indices.size());
		vao_unbind();
	}
}