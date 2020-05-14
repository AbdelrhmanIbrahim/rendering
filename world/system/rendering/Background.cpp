#include "world/system/rendering/Background.h"
#include "world/component/Camera.h"

#include "math/Vector.h"
#include "math/Matrix.h"

#include "world/component/Vertex.h"

using namespace math;

namespace world
{
	namespace system
	{
		void
		bg_sys_run(rndr::Cquad self, ecs::World& w)
		{
			//fetch system req components
			Camera cam = ecs::world_active_components<world::Camera>(w)[0];
			Mat4f vp = camera_proj_matrix(cam);
			vec2f viewport = world::camera_viewport(cam);

			//covert the whole view
			CVertex tl {vec3f{ 2*cam.l, 2*cam.t, cam.f }, math::vec4f{0.7, 0.7, 0.7, 1}};
			CVertex bl {vec3f{ 2*cam.l, 2*cam.b, cam.f }, math::vec4f{0.2, 0.2, 0.2, 1}};
			CVertex br {vec3f{ 2*cam.r, 2*cam.b, cam.f }, math::vec4f{0.2, 0.2, 0.2, 1}};
			CVertex tr {vec3f{ 2*cam.r, 2*cam.t, cam.f }, math::vec4f{0.7, 0.7, 0.7, 1}};
			CVertex quad[6]
			{
				tl,bl,br,
				tl,br,tr
			};

			cquad_set(self, vp, viewport);
			cquad_draw(self, quad);
		}
	};
};