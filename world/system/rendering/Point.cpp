#include "world/system/rendering/Point.h"
#include "world/component/Camera.h"
#include "world/component/Lamp.h"

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
			auto cam = ecs::world_active_components<world::Camera>(w)[0];
			auto lamps = ecs::world_active_components<world::Lamp>(w);

			//append some points
			for (int i = 0; i < lamps.size; ++i)
			{
				rndr::point_append(sys.point,
					math::vec3f{ lamps[i].pos[0], lamps[i].pos[1] , lamps[i].pos[2] },
					lamps[i].col);
			}

			rndr::point_set(sys.point, camera_view_proj(cam));
			rndr::point_draw(sys.point);
		}

		void
		point_free(Point_System sys)
		{
			rndr::point_free(sys.point);
		}
	};
};