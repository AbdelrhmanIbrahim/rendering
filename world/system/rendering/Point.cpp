#include "world/system/rendering/Point.h"
#include "world/component/Camera.h"

#include "math/Vector.h"

namespace world
{
	namespace system
	{
		Point_System
		point_new()
		{
			Point_System sys{};
			sys.point = rndr::point_create();

			return sys;
		}

		void
		point_run(Point_System sys, ecs::World& w)
		{
			//fetch system req components
			auto& cam = ecs::world_active_components<world::Camera>(w)[0];

			//append some points
			rndr::point_append(sys.point, math::vec3f{0,1,0});

			//set color and data to be drawn then draw
			rndr::point_set(sys.point, camera_view_proj(cam), math::vec4f{ 1,0,0,1 });
			rndr::point_draw(sys.point);
		}

		void
		point_free(Point_System sys)
		{
			rndr::point_free(sys.point);
		}
	};
};