#include "engine/renderer/Quad.h"

#include "ecs/World.h"

namespace world
{
	namespace system
	{
		//colored quad as a bg
		void
		bg_sys_run(rndr::Cquad self, ecs::World& w);
	};
};