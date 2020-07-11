#include "world/system/rendering/Point.h"
#include "world/component/Camera.h"
#include "world/component/Lamp.h"

#include "math/Vector.h"

#include "world/component/Vertex.h"

namespace world
{
	namespace system
	{
		void
		point_sys_run(rndr::Point point, ecs::World& w)
		{
			//fetch system req components
			auto cam = ecs::world_active_components<world::Camera>(w)[0];
			auto lamps = ecs::world_active_components<world::Lamp>(w);

			//append some points
			for (int i = 0; i < lamps.size; ++i)
				rndr::point_append(point, world::CVertex{math::Vec3f{ lamps[i].pos[0], lamps[i].pos[1] , lamps[i].pos[2] },lamps[i].col});

			rndr::point_set(point, camera_view_proj(cam));
			rndr::point_draw(point);
		}
	};
};