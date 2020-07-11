#include "world/system/rendering/Hiddenline.h"

#include "world/component/Camera.h"
#include "world/component/Mesh.h"
#include "world/component/Transform.h"

#include "math/Vector.h"

namespace world
{
	namespace system
	{
		void
		hiddenline_sys_run(rndr::Hiddenline hline, ecs::World& w)
		{
			//fetch system req components
			auto cam = ecs::world_active_components<world::Camera>(w)[0];
			auto b_meshes = ecs::world_active_components<world::Mesh>(w);
			auto b_transforms = ecs::world_active_components<world::Transform>(w);
			math::Vec2f viewport = world::camera_viewport(cam);
			math::Mat4f vp = camera_view_proj(cam);

			rndr::hiddenline_set(hline, viewport);
			for (int i = 0; i < b_meshes.size; ++i)
				rndr::hiddenline_draw(hline, vp, &b_meshes[i], &b_transforms[i], math::Vec4f{ 1, 1, 1, 1 });
		}
	};
};