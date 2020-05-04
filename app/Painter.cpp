#include "app/Painter.h"

#include "io/Input.h"
#include "io/Event.h"

#include "engine/Engine.h"

#include "world/Universe.h"
#include "world/system/querying/Pick.h"

#include "math/Vector.h"

#include "infra/managers/Selector.h"

using namespace rndr;
using namespace io;
using namespace infra;

namespace app
{
	struct IPainter
	{
		//input state
		io::Input input;

		//rendering engine
		rndr::Engine engine;

		//universe
		world::Universe universe;

		//querying systems
		world::system::Pick_System pick_sys;

		//selection manager
		manager::Selector selector;
	};

	//API
	Painter
	painter_new()
	{
		IPainter* app = new IPainter;

		//init input state
		app->input = io::input_new();

		//init rendering engine (init rendering engine and its context first as universe may have components seperated into cpu and gpu alloc)
		app->engine = engine_create();

		//init universe
		app->universe = world::universe_new();

		//init quering sys
		app->pick_sys = world::system::pick_sys_new();

		//init selector
		app->selector = manager::selector_new();

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
		return world::universe_3dobject_add(app->universe, path);
	}

	void
	painter_input(Painter app, Event event)
	{
		input_process_event(app->input, event);
	}

	void
	painter_update(Painter app, int window_width, int window_height)
	{
		//Run updating data systems
		//universe update system
		world::universe_update_sys_run(app->universe, math::vec2f{ (float)window_width, (float)window_height }, app->input);

		//painter selection system
		if (app->input.mouse[0] == true)
		{
			ecs::Entity selected_entity = world::system::pick_system_run(app->pick_sys, app->universe.world, app->input, rndr::engine_colored_renderer(app->engine));
			manager::selector_select(app->selector, selected_entity);
		}

		//universe moving selected entities system
		world::universe_movement_sys_run(app->universe, app->input, app->selector.selected_entity);

		//universe script system must run after all internal data gets updated by other systems
		world::universe_scripts_sys_run(app->universe);
		input_update(app->input);
	}

	void
	painter_paint(Painter app, void* palette, unsigned int width, unsigned int height)
	{
		//Run rendering systems
		engine_world_draw(app->engine, app->universe.world, palette);
		engine_imgui_draw(app->engine, math::vec2f{ (float)app->input.mouse_x, (float)app->input.mouse_y}, app->input.mouse, palette, width, height);
		win::window_swap(palette);
	}

	void
	painter_free(Painter app)
	{
		rndr::engine_free(app->engine);
		world::universe_free(app->universe);
		world::system::pick_sys_free(app->pick_sys);

		delete app;
	}
};