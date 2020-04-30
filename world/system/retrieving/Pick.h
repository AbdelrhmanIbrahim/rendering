#pragma once

#include "ecs/World.h"

#include "engine/renderer/Colored.h"

#include "gl/glgpu.h"

#include "math/Vector.h"
#include "math/Matrix.h"

#include "io/Input.h"

namespace world
{
	namespace system
	{
		//gpu picking
		struct Pick_System
		{
			glgpu::Framebuffer fb;
			glgpu::Texture tex;
		};

		Pick_System
		pick_sys_new();

		void
		pick_system_run(Pick_System sys, ecs::World& w, io::Input& i, rndr::Colored colored);

		void
		pick_sys_free(Pick_System sys);
	}
}