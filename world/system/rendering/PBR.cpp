#include "world/system/rendering/PBR.h"

#include "world/component/Camera.h"
#include "world/component/Mesh.h"
#include "world/component/Transform.h"
#include "world/component/Material.h"
#include "world/component/Sun.h"
#include "world/component/Lamp.h"
#include "world/component/Flash.h"

#include "infra/mem/chunck.h"

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
			//fetch system req components
			auto cam = ecs::world_active_components<world::Camera>(w)[0];
			auto b_meshes = ecs::world_active_components<world::Mesh>(w);
			auto b_transforms = ecs::world_active_components<world::Transform>(w);
			auto b_materials = ecs::world_active_components<world::Material>(w);
			auto b_suns = ecs::world_active_components<world::Sun>(w);
			auto b_lamps = ecs::world_active_components<world::Lamp>(w);
			auto b_flashes = ecs::world_active_components<world::Flash>(w);

			math::vec2f viewport = world::camera_viewport(cam);
			rndr::pbr_set(sys.pbr, 
				&cam,
				infra::mem::chunk<world::Sun>{ b_suns.size, b_suns.ptr },
				infra::mem::chunk<world::Lamp>{ b_lamps.size, b_lamps.ptr },
				infra::mem::chunk<world::Flash>{ b_flashes.size, b_flashes.ptr });

			for (int i = 0; i < b_meshes.size; ++i)
				rndr::pbr_draw(sys.pbr, camera_view_proj(cam), &b_meshes[i], &b_transforms[i], &b_materials[i]);
		}

		void
		pbr_free(PBR_System sys)
		{
			rndr::pbr_free(sys.pbr);
		}
	};
};