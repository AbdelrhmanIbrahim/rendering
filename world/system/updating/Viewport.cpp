#include "world/system/updating/Viewport.h"
#include "world/component/Camera.h"

namespace world
{
	namespace system
	{
		void
		viewport_sys_run(ecs::World& w, io::Input& i, math::vec2f win_size)
		{
			//fetch system req components
			auto cam_bag = ecs::world_active_components<world::Camera>(w);
			for (int x = 0; x < cam_bag.size; ++x)
				camera_viewport(cam_bag[x], win_size);
		}
	};
};
