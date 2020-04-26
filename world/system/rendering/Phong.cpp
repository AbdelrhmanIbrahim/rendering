#include "world/system/rendering/Phong.h"

#include "world/component/Camera.h"
#include "world/component/Mesh.h"
#include "world/component/Transform.h"
#include "world/component/Material.h"
#include "world/component/Sun.h"
#include "world/component/Lamp.h"
#include "world/component/Flash.h"

namespace world
{
	namespace system
	{
		void
		phong_render(rndr::Phong phong, ecs::World& w)
		{
			//get bags
			auto b_cam = ecs::world_components_data<world::Camera>(w)[0].data;
			auto b_meshes = ecs::world_components_data<world::Mesh>(w);
			auto b_transforms = ecs::world_components_data<world::Transform>(w);
			auto b_materials = ecs::world_components_data<world::Material>(w);
			auto b_suns = ecs::world_components_data<world::Sun>(w);
			auto b_lamps = ecs::world_components_data<world::Lamp>(w);
			auto b_flashes = ecs::world_components_data<world::Flash>(w);

			//lighting setting (unnecassery data transformation here -revisit-)
			std::vector<world::Sun> suns;
			for (int i = 0; i < b_suns.size; ++i)
			{
				if (b_suns[i].deleted == false)
					suns.emplace_back(b_suns[i].data);
			}

			std::vector<world::Lamp> lamps;
			for (int i = 0; i < b_lamps.size; ++i)
			{
				if (b_lamps[i].deleted == false)
					lamps.emplace_back(b_lamps[i].data);
			}

			std::vector<world::Flash> flashes;
			for (int i = 0; i < b_flashes.size; ++i)
			{
				if (b_flashes[i].deleted == false)
					flashes.emplace_back(b_flashes[i].data);
			}
			phong_set(phong, &b_cam, suns, lamps, flashes);

			for (int i = 0; i < b_meshes.size; ++i)
			{
				if (b_meshes[i].deleted == false)
					phong_draw(phong, camera_view_proj(b_cam), &b_meshes[i].data, &b_transforms[i].data, &b_materials[i].data);
			}
		}
	};
};