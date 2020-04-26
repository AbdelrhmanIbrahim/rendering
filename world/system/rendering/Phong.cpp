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
			auto& c_cam = ecs::world_components_data<world::Camera>(w).front().data;
			auto& c_meshes = ecs::world_components_data<world::Mesh>(w);
			auto& c_transforms = ecs::world_components_data<world::Transform>(w);
			auto& c_materials = ecs::world_components_data<world::Material>(w);
			auto& c_suns = ecs::world_components_data<world::Sun>(w);
			auto& c_lamps = ecs::world_components_data<world::Lamp>(w);
			auto& c_flashes = ecs::world_components_data<world::Flash>(w);

			//lighting setting (unnecassery data transformation here -revisit-)
			std::vector<world::Sun> suns;
			for (int i = 0; i < c_suns.size(); ++i)
			{
				if (c_suns[i].deleted == false)
					suns.emplace_back(c_suns[i].data);
			}

			std::vector<world::Lamp> lamps;
			for (int i = 0; i < c_lamps.size(); ++i)
			{
				if (c_lamps[i].deleted == false)
					lamps.emplace_back(c_lamps[i].data);
			}

			std::vector<world::Flash> flashes;
			for (int i = 0; i < c_flashes.size(); ++i)
			{
				if (c_flashes[i].deleted == false)
					flashes.emplace_back(c_flashes[i].data);
			}
			phong_set(phong, &c_cam, suns, lamps, flashes);

			for (int i = 0; i < c_meshes.size(); ++i)
			{
				if (c_meshes[i].deleted == false)
					phong_draw(phong, camera_view_proj(c_cam), &c_meshes[i].data, &c_transforms[i].data, &c_materials[i].data);
			}
		}
	};
};