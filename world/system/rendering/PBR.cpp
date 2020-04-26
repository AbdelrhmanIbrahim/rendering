#include "world/system/rendering/PBR.h"

#include "world/component/Camera.h"
#include "world/component/Mesh.h"
#include "world/component/Transform.h"
#include "world/component/Material.h"

#include "math/Vector.h"

namespace world
{
	namespace system
	{
		PBR_System
		pbr_new()
		{
			PBR_System self{};
			self.pbr = rndr::pbr_create();
			return self;
		}

		void
		pbr_run(PBR_System sys, ecs::World& w)
		{
			//get bags
			auto cam = ecs::world_active_components_entities<world::Camera>(w)[0].data;
			auto b_meshes = ecs::world_active_components_entities<world::Mesh>(w);
			auto b_transforms = ecs::world_active_components_entities<world::Transform>(w);
			auto b_materials = ecs::world_active_components_entities<world::Material>(w);

			math::vec2f viewport = world::camera_viewport(cam);
			rndr::pbr_set(sys.pbr, &cam);
			for (int i = 0; i < b_meshes.size; ++i)
				rndr::pbr_draw(sys.pbr, camera_view_proj(cam), &b_meshes[i].data, &b_transforms[i].data, &b_materials[i].data);
		}

		void
		pbr_free(PBR_System sys)
		{
			rndr::pbr_free(sys.pbr);
		}
	};
};