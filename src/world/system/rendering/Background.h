#include "engine/renderer/CQuad.h"
#include "engine/renderer/TQuad.h"

#include "ecs/World.h"

#include "gl/glgpu.h"

namespace world
{
	namespace system
	{
		//colored quad as bg
		void
		bg_col_sys_run(rndr::CQuad self, ecs::World& w);

		//image quad as bg
		void
		bg_img_sys_run(rndr::TQuad self, ecs::World& w);
	};
};