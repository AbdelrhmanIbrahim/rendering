#include "world/system/rendering/Skybox.h"

#include "world/component/Camera.h"

#include "math/Vector.h"

namespace world
{
	namespace system
	{
		void
		skybox_run(rndr::Skybox skybox, ecs::World& w)
		{
			//fetch system req components
			auto cam = ecs::world_active_components<world::Camera>(w)[0];
			rndr::skybox_renderer_draw(skybox, &cam);
		}
	};
};