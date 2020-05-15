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
		//revisit
		constexpr CVertex tl{ vec3f{ -1,  1, 1 }, math::vec4f{0.7, 0.7, 0.7, 1} };
		constexpr CVertex bl{ vec3f{ -1, -1, 1 }, math::vec4f{0.2, 0.2, 0.2, 1} };
		constexpr CVertex br{ vec3f{  1, -1, 1 }, math::vec4f{0.2, 0.2, 0.2, 1} };
		constexpr CVertex tr{ vec3f{  1,  1, 1 }, math::vec4f{0.7, 0.7, 0.7, 1} };
		constexpr CVertex bg_quad[6]
		{
			tl,bl,br,
			tl,br,tr
		};

		void
		bg_sys_run(rndr::Cquad self, ecs::World& w)
		{
			//fetch system req components
			Camera cam = ecs::world_active_components<world::Camera>(w)[0];
			cquad_set(self, mat4_id(), world::camera_viewport(cam));
			cquad_draw(self, bg_quad);
		}
	};
};