#pragma once

//the palette which the engine will draw to
#include "window/Window.h"

//for imgui, revisit later
#include "io/Input.h"

namespace world
{
	struct World;
}

namespace rndr
{
	typedef struct IEngine* Engine;

	Engine
	engine_create();

	void
	engine_free(Engine e);

	void
	engine_world_draw(Engine e, const world::World* w, win::Window palette);

	void
	engine_imgui_draw(Engine e, const io::Input& app_i, win::Window palette);
}