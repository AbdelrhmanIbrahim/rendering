#include "world/system/rendering/Colored.h"

#include "world/component/Camera.h"
#include "world/component/Mesh.h"
#include "world/component/Transform.h"
#include "world/component/Material.h"

#include "math/Vector.h"

namespace world
{
	namespace system
	{
		void
		colored_sys_run(rndr::Colored colored, ecs::World& w)
		{
			//fetch system req components
			auto cam = ecs::world_active_components<world::Camera>(w)[0];
			auto b_meshes = ecs::world_active_components<world::Mesh>(w);
			auto b_transforms = ecs::world_active_components<world::Transform>(w);
			auto b_materials = ecs::world_active_components<world::Material>(w);

			math::Vec2f viewport = world::camera_viewport(cam);
			rndr::colored_set(colored, viewport);

			math::Mat4f vp = camera_view_proj(cam);
			for (int i = 0; i < b_meshes.size; ++i)
				rndr::colored_draw(colored, vp, &b_meshes[i], &b_transforms[i], b_materials[i].color_norm);
		}
	};
};