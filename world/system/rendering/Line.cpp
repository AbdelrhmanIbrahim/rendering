#include "world/system/rendering/Line.h"
#include "world/component/Camera.h"

#include "math/Vector.h"

namespace world
{
	namespace system
	{
		Line_System
		line_new()
		{
			Line_System sys{};
			sys.point = rndr::line_create();

			return sys;
		}

		void
		line_run(Line_System sys, ecs::World& w)
		{
			//fetch system req components
			auto cam = ecs::world_active_components<world::Camera>(w)[0];

			//draw world ref axes
			rndr::line_append(sys.point, 
							Pnt{ math::vec3f{0, 0, 0}, math::vec4f{1,0,0,1} },
							Pnt{ math::vec3f{1, 0, 0}, math::vec4f{1,0,0,1} });

			rndr::line_append(sys.point,
							Pnt{ math::vec3f{0, 0, 0}, math::vec4f{0,1,0,1} },
							Pnt{ math::vec3f{0, 1, 0}, math::vec4f{0,1,0,1} });

			rndr::line_append(sys.point,
						Pnt{ math::vec3f{0, 0, 0}, math::vec4f{0,0,1,1} },
						Pnt{ math::vec3f{0, 0, 1}, math::vec4f{0,0,1,1} });

			rndr::line_set(sys.point, camera_view_proj(cam));
			rndr::line_draw(sys.point);
		}

		void
		line_free(Line_System sys)
		{
			rndr::line_free(sys.point);
		}
	};
};