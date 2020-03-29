#include "app/Painter.h"

#include "defs/Defs.h"

#include "io/Input.h"
#include "io/Event.h"

#include "math/Vector.h"

#include "world/World.h"
#include "engine/Engine.h"

#include "world/ECS_World.h"

using namespace world;
using namespace rndr;
using namespace io;

namespace app
{
	struct IPainter
	{
		//input state
		io::Input i;

		//rendering engine and world
		rndr::Engine e;
		world::World* w;
		
		ecs::World ecs_w;

		//check if app is running or not
		bool is_running;
	};

	//internal helpers
	void
	_input_act(const Input& i, World* w)
	{
		//Mouse move
		{
			math::vec2f mouse_dir{ input_mouse_delta(i) };
			if (mouse_dir != math::vec2f{})
				camera_rotate(w->cam, mouse_dir);
		}

		//Keyboard
		{
			constexpr float speed = 0.005f * 2.0f;
			if (i.keyboard[(int)io::KEYBOARD::W] == true)
				camera_move_forward(w->cam, speed);
			if (i.keyboard[(int)io::KEYBOARD::S] == true)
				camera_move_backward(w->cam, speed);
			if (i.keyboard[(int)io::KEYBOARD::A] == true)
				camera_move_left(w->cam, speed);
			if (i.keyboard[(int)io::KEYBOARD::D] == true)
				camera_move_right(w->cam, speed);
		}

		//Wheel
		{
			camera_zoom(w->cam, i.wheel_dir);
		}
	}

	//API
	Painter
	painter_new()
	{
		IPainter* app = new IPainter;

		//app window
		app->is_running = true;

		//input init state
		app->i = Input{};
		app->i.mouse_x = WIN_WIDTH / 2;
		app->i.mouse_y = WIN_HEIGHT / 2;
		app->i.pmouse_x = WIN_WIDTH / 2;
		app->i.pmouse_y = WIN_HEIGHT / 2;

		//init rendering engine and world
		app->e = engine_create();
		app->w = world_create();

		//init testing ecs world
		app->ecs_w = world::_world_create();

		return app;
	}

	void
	painter_input(Painter app, Event event)
	{
		if (event.kind == Event::KIND::KIND_WINDOW_CLOSE)
		{
			app->is_running = false;
			return;
		}

		input_process_event(app->i, event);
	}

	void
	painter_update(Painter app, int window_width, int window_height)
	{
		_input_act(app->i, app->w);
		input_mouse_update(app->i);
		camera_viewport(app->w->cam, math::vec2f{(float)window_width, (float)window_height});
	}

	void
	painter_paint(Painter app, void* palette, unsigned int width, unsigned int height)
	{
		engine_world_draw(app->e, app->w, palette);
		engine_imgui_draw(app->e, app->i, palette, width, height);
		win::window_swap(palette);
	}

	bool
	painter_drawing(Painter app)
	{
		return app->is_running;
	}

	void
	painter_free(Painter app)
	{
		world_free(app->w);
		engine_free(app->e);

		_world_free(app->ecs_w);

		delete app;
	}
};