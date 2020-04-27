#include "app/Painter.h"

#include "defs/Defs.h"

#include "io/Input.h"
#include "io/Event.h"

#include "math/Vector.h"

#include "engine/Engine.h"

#include "world/Universe.h"
#include "world/component/Camera.h"
#include "world/component/Flash.h"

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

	//Internal helpers
	void
	_input_act(const Input& i, world::Camera& cam, world::Flash& flash)
	{
		//Mouse move
		{
			math::vec2f mouse_dir{ input_mouse_delta(i) };
			if (mouse_dir != math::vec2f{})
			{
				camera_rotate(cam, mouse_dir);

				//till we have a rigid communication and relations between components
				flash.dir = math::vec4f{ cam.fwd[0], cam.fwd[1], cam.fwd[2], 0 };
			}
		}

		//Keyboard
		{
			constexpr float speed = 0.05f * 2.0f;
			if (i.keyboard[(int)io::KEYBOARD::W] == true)
				camera_move_forward(cam, speed);
			if (i.keyboard[(int)io::KEYBOARD::S] == true)
				camera_move_backward(cam, speed);
			if (i.keyboard[(int)io::KEYBOARD::A] == true)
				camera_move_left(cam, speed);
			if (i.keyboard[(int)io::KEYBOARD::D] == true)
				camera_move_right(cam, speed);

			//till we have a rigid communication and relations between components
			flash.pos = math::vec4f{ cam.pos[0], cam.pos[1], cam.pos[2], 0 };
		}

		//Wheel
		{
			camera_zoom(cam, i.wheel_dir);
		}
	}

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
		//this is for now till we have some good way for communicating between components
		auto& cam = ecs::world_active_components<world::Camera>(app->universe.world)[0];
		auto& cam_flash = ecs::world_active_components<world::Flash>(app->universe.world)[0];

		_input_act(app->input, cam, cam_flash);
		input_mouse_update(app->input);
		world::camera_viewport(cam, math::vec2f{ (float)window_width, (float)window_height });

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