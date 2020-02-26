#include "app/App.h"

#include "Defs/Defs.h"

#include "IO/Input.h"

#include "math/Vector.h"

#include "window/Window.h"
#include "gpu_gl/gl_context.h"

#include "world/World.h"
#include "engine/Engine.h"

using namespace world;
using namespace rndr;
using namespace io;

namespace app
{
	struct IApp
	{
		//win and ogl context
		math::vec2f window_size;
		win::Window win;
		glgpu::Context ctx;

		//input state
		io::Input i;

		//rendering engine and world
		rndr::Engine e;
		world::World* w;
	};


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

	App
	app_new()
	{
		IApp* app = new IApp;

		//window and its context
		app->window_size = math::vec2f{ WIN_WIDTH, WIN_HEIGHT };
		app->win = win::window_new(WIN_WIDTH, WIN_HEIGHT, "rendering journey");
		app->ctx = glgpu::context_attach(4, 0, app->win);

		//input init state
		app->i = Input{};
		app->i.mouse_x = WIN_WIDTH / 2;
		app->i.mouse_y = WIN_HEIGHT / 2;
		app->i.pmouse_x = WIN_WIDTH / 2;
		app->i.pmouse_y = WIN_HEIGHT / 2;

		app->e = engine_create();
		app->w = world_create();

		return app;
	}

	void
	app_run(App app)
	{
		while (true)
		{
			//get the window input event
			auto event = win::window_poll(app->win);

			//resize or close window
			if (event.kind == win::Window_Event::KIND::KIND_WINDOW_RESIZE)
			{
				app->window_size[0] = event.window_resize.width;
				app->window_size[1] = event.window_resize.height;
			}
			else if (event.kind == win::Window_Event::KIND::KIND_WINDOW_CLOSE)
				break;

			//send event to input
			input_process_event(app->i, event);

			//call the right procedures according to the input state to update the data
			_input_act(app->i, app->w);
			input_mouse_update(app->i);

			//render the data
			{
				//set camera viewport to window's viewport
				camera_viewport(app->w->cam, app->window_size);

				//render
				engine_world_draw(app->e, app->w);
			}

			//swap window buffers
			window_swap(app->win);
		}
	}

	void
	app_free(App app)
	{
		world_free(app->w);
		engine_free(app->e);

		win::window_free(app->win);
		glgpu::context_free(app->ctx);

		delete app;
	}

};