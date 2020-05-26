#include "engine/renderer/CQuad.h"

#include "ecs/World.h"

namespace world
{
	namespace system
	{
		//colored quad as a bg
		void
		bg_sys_run(rndr::CQuad self, ecs::World& w);
	};
};