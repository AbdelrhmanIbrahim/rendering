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
		colored_render(rndr::Colored colored, ecs::World& w)
		{
			//get bags
			auto cam = ecs::world_components_data<world::Camera>(w)[0].data;
			auto b_meshes = ecs::world_components_data<world::Mesh>(w);
			auto b_transforms = ecs::world_components_data<world::Transform>(w);
			auto b_materials = ecs::world_components_data<world::Material>(w);

			math::vec2f viewport = world::camera_viewport(cam);
			rndr::colored_set(colored, viewport);
			for (int i = 0; i < b_meshes.size; ++i)
			{
				if (b_meshes[i].deleted == false)
					rndr::colored_draw(colored, camera_view_proj(cam), &b_meshes[i].data, &b_transforms[i].data, b_materials[i].data.color_norm);
			}
		}
	};
};