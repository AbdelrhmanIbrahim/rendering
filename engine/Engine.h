#pragma once

#include "ecs/World.h"

#include "math/Vector.h"

#include "defs/Defs.h"

#include "engine/renderer/Colored.h"

namespace rndr
{
	typedef struct IEngine* Engine;

	Engine
	engine_create();

	void
	engine_free(Engine e);

	void
	engine_rendering_style(Engine e, Rendering mode);

	void
	engine_world_draw(Engine e, ecs::World& w, void* win);

	void
	engine_imgui_draw(Engine e, math::vec2f mouse_pos, bool mouse[3], void* win, unsigned int width, unsigned int height);

	//think of a better way? -revisit-
	rndr::Colored
	engine_colored_renderer(Engine e);
}