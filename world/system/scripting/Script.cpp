#include "world/system/scripting/Script.h"

namespace world
{
	namespace system
	{
		void
		script_system_run(Script_System sys, ecs::World& w)
		{
			for (const auto& script : sys.cam_flash_scripts)
				ecs::script_run(script, w);
		}

		void
		script_flash_cam(world::Camera* cam, world::Flash* flash)
		{

		}
	};
};
