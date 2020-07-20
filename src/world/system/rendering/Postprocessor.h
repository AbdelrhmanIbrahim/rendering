#pragma once

#include "ecs/World.h"

#include "engine/renderer/Postprocessor.h"

#include "gl/glgpu.h"

namespace world
{
	namespace system
	{
		glgpu::Texture
		postprocess_sys_run(rndr::Postprocessor pp, ecs::World& w, glgpu::Texture in);
	};
};