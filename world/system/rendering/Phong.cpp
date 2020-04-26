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
		Phong_System
		phong_new()
		{
			Phong_System self{};
			self.phong = rndr::phong_create();
			return self;
		}

		void
		phong_run(Phong_System sys, ecs::World& w)
		{
			//get bags
			auto b_cam = ecs::world_active_components_entities<world::Camera>(w)[0].data;
			auto b_meshes = ecs::world_active_components_entities<world::Mesh>(w);
			auto b_transforms = ecs::world_active_components_entities<world::Transform>(w);
			auto b_materials = ecs::world_active_components_entities<world::Material>(w);
			auto b_suns = ecs::world_active_components_entities<world::Sun>(w);
			auto b_lamps = ecs::world_active_components_entities<world::Lamp>(w);
			auto b_flashes = ecs::world_active_components_entities<world::Flash>(w);

			//lighting setting (unnecassery data transformation here -revisit-)
			std::vector<world::Sun> suns;
			for (int i = 0; i < b_suns.size; ++i)
				suns.emplace_back(b_suns[i].data);

			std::vector<world::Lamp> lamps;
			for (int i = 0; i < b_lamps.size; ++i)
				lamps.emplace_back(b_lamps[i].data);

			std::vector<world::Flash> flashes;
			for (int i = 0; i < b_flashes.size; ++i)
				flashes.emplace_back(b_flashes[i].data);

			phong_set(sys.phong, &b_cam, suns, lamps, flashes);

			for (int i = 0; i < b_meshes.size; ++i)
				phong_draw(sys.phong, camera_view_proj(b_cam), &b_meshes[i].data, &b_transforms[i].data, &b_materials[i].data);
		}

		void
		phong_free(Phong_System sys)
		{
			rndr::phong_free(sys.phong);
		}
	};
};