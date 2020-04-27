#include "app/Painter.h"

#include "defs/Defs.h"

#include "io/Input.h"
#include "io/Event.h"

#include "math/Vector.h"

#include "engine/Engine.h"

#include "world/Universe.h"

using namespace world;
using namespace rndr;
using namespace io;

namespace app
{
	struct IPainter
	{
		//input state
		io::Input input;

		//rendering engine and universe
		Rendering style;
		rndr::Engine engine;
		world::Universe universe;
	};

	//API
	Painter
	painter_new()
	{
		IPainter* app = new IPainter;

		//input init state
		app->input = Input{};
		app->input.mouse_x = WIN_WIDTH / 2;
		app->input.mouse_y = WIN_HEIGHT / 2;
		app->input.pmouse_x = WIN_WIDTH / 2;
		app->input.pmouse_y = WIN_HEIGHT / 2;

		//init rendering engine and world
		app->engine = engine_create();

		//init testing universe scene
		app->universe = world::universe_new();
		world::universe_init_scene(app->universe);

		return app;
	}

	void
	painter_style(Painter app, Rendering mode)
	{
		engine_rendering_style(app->engine, mode);
	}

	bool
	painter_stl_load(Painter app, const char* path)
	{
		return world::universe_load_stl(app->universe, path);
	}

	void
	painter_input(Painter app, Event event)
	{
		input_process_event(app->input, event);
	}

	void
	painter_update(Painter app, int window_width, int window_height)
	{
		universe_input_act(app->universe, app->input, window_width, window_height);
		input_mouse_update(app->input);
		universe_scripts_run(app->universe);
	}

	void
	painter_paint(Painter app, void* palette, unsigned int width, unsigned int height)
	{
		engine_world_draw(app->engine, app->universe.world, palette);
		engine_imgui_draw(app->engine, math::vec2f{ (float)app->input.mouse_x, (float)app->input.mouse_y}, app->input.mouse, palette, width, height);
		win::window_swap(palette);
	}

	void
	painter_free(Painter app)
	{
		rndr::engine_free(app->engine);
		world::universe_free(app->universe);

		delete app;
	}
};