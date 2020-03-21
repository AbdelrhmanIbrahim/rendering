#include "app/App.h"

#include "defs/Defs.h"

#include "io/Input.h"

#include "math/Vector.h"

#include "world/World.h"
#include "engine/Engine.h"

using namespace world;
using namespace rndr;
using namespace io;

namespace app
{
	struct IApp
	{
		//input state
		io::Input i;

		//rendering engine and world
		rndr::Engine e;
		world::World* w;

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
			if (i.keyboard[(int)win::KEYBOARD::W] == true)
				camera_move_forward(w->cam, speed);
			if (i.keyboard[(int)win::KEYBOARD::S] == true)
				camera_move_backward(w->cam, speed);
			if (i.keyboard[(int)win::KEYBOARD::A] == true)
				camera_move_left(w->cam, speed);
			if (i.keyboard[(int)win::KEYBOARD::D] == true)
				camera_move_right(w->cam, speed);
		}

		//Wheel
		{
			camera_zoom(w->cam, i.wheel_dir);
		}
	}

	//API
	App
	app_new()
	{
		IApp* app = new IApp;

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

		return app;
	}

	void
	app_input(App app, win::Window_Event event)
	{
		if (event.kind == win::Window_Event::KIND::KIND_WINDOW_CLOSE)
		{
			app->is_running = false;
			return;
		}

		input_process_event(app->i, event);
	}

	void
	app_update(App app, int window_width, int window_height)
	{
		_input_act(app->i, app->w);
		input_mouse_update(app->i);
		camera_viewport(app->w->cam, math::vec2f{(float)window_width, (float)window_height});
	}

	void
	app_paint(App app, win::Window palette)
	{
		engine_world_draw(app->e, app->w, palette);
		engine_imgui_draw(app->e, app->i, palette);
		window_swap(palette);
	}

	bool
	app_running(App app)
	{
		return app->is_running;
	}

	void
	app_free(App app)
	{
		world_free(app->w);
		engine_free(app->e);

		delete app;
	}
};