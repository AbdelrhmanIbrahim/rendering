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
		Colored_System
		colored_new()
		{
			Colored_System self{};
			self.colored = rndr::colored_create();
			return self;
		}

		void
		colored_run(Colored_System sys, ecs::World& w)
		{
			//fetch system req components
			auto cam = ecs::world_active_components<world::Camera>(w)[0];
			auto b_meshes = ecs::world_active_components<world::Mesh>(w);
			auto b_transforms = ecs::world_active_components<world::Transform>(w);
			auto b_materials = ecs::world_active_components<world::Material>(w);

			math::vec2f viewport = world::camera_viewport(cam);
			rndr::colored_set(sys.colored, viewport);
			for (int i = 0; i < b_meshes.size; ++i)
				rndr::colored_draw(sys.colored, camera_view_proj(cam), &b_meshes[i], &b_transforms[i], b_materials[i].color_norm);
		}

		void
		colored_free(Colored_System sys)
		{
			rndr::colored_free(sys.colored);
		}
	};
};