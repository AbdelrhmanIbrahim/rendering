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
			flash->dir = math::vec4f{ cam->fwd[0], cam->fwd[1], cam->fwd[2], 0 };
			flash->pos = math::vec4f{ cam->pos[0], cam->pos[1], cam->pos[2], 0 };
		}
	};
};
