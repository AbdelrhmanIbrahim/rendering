#pragma once

#include "ecs/World.h"
#include "ecs/Script.h"

#include "world/component/Camera.h"
#include "world/component/Flash.h"

#include <vector>

namespace world
{
	namespace system
	{
		struct
		Script_System
		{
			std::vector<ecs::Script<world::Camera, world::Flash>> cam_flash_scripts;
		};

		void
		script_system_run(Script_System sys, ecs::World& w);

		//scripts
		void
		script_flash_cam(world::Camera* cam, world::Flash* flash);
	}
}