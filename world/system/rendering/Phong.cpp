#include "world/system/rendering/Phong.h"

#include "world/component/Camera.h"
#include "world/component/Mesh.h"
#include "world/component/Transform.h"
#include "world/component/Material.h"
#include "world/component/Sun.h"
#include "world/component/Lamp.h"
#include "world/component/Flash.h"

#include "infra/mem/chunck.h"

namespace world
{
	namespace system
	{
		void
		phong_run(rndr::Phong phong, ecs::World& w)
		{
			//fetch system req components
			auto cam = ecs::world_active_components<world::Camera>(w)[0];
			auto b_meshes = ecs::world_active_components<world::Mesh>(w);
			auto b_transforms = ecs::world_active_components<world::Transform>(w);
			auto b_materials = ecs::world_active_components<world::Material>(w);
			auto b_suns = ecs::world_active_components<world::Sun>(w);
			auto b_lamps = ecs::world_active_components<world::Lamp>(w);
			auto b_flashes = ecs::world_active_components<world::Flash>(w);

			phong_set(phong, &cam,
				infra::mem::chunk<world::Sun>{ b_suns.size, b_suns.ptr },
				infra::mem::chunk<world::Lamp>{ b_lamps.size, b_lamps.ptr },
				infra::mem::chunk<world::Flash> { b_flashes.size, b_flashes.ptr });

			math::Mat4f vp = camera_view_proj(cam);
			for (int i = 0; i < b_meshes.size; ++i)
				phong_draw(phong, vp, &b_meshes[i], &b_transforms[i], &b_materials[i]);
		}
	};
};