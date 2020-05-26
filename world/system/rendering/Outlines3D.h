#include "engine/renderer/Colored.h"
#include "engine/renderer/TQuad.h"

#include "ecs/World.h"

namespace world
{
	namespace system
	{
		//draw outlines of 3d object (selected)
		void
		outline3d_sys_run(rndr::Colored colored, rndr::TQuad tquad,  ecs::World& w, ecs::Entity selected_id);
	};
};