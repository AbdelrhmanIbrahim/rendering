#include "app/Painter.h"

#include "defs/Defs.h"

#include "io/Input.h"
#include "io/Event.h"

#include "math/Vector.h"

#include "engine/Engine.h"

#include "ecs/World.h"
#include "world/components/Transform.h"
#include "world/components/Mesh.h"
#include "world/components/Camera.h"

using namespace world;
using namespace rndr;
using namespace ecs;
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
	__input_act(const Input& i, world::Camera& cam)
	{
		//Mouse move
		{
			math::vec2f mouse_dir{ input_mouse_delta(i) };
			if (mouse_dir != math::vec2f{})
				camera_rotate(cam, mouse_dir);
		}

		//Keyboard
		{
			constexpr float speed = 0.005f * 2.0f;
			if (i.keyboard[(int)io::KEYBOARD::W] == true)
				camera_move_forward(cam, speed);
			if (i.keyboard[(int)io::KEYBOARD::S] == true)
				camera_move_backward(cam, speed);
			if (i.keyboard[(int)io::KEYBOARD::A] == true)
				camera_move_left(cam, speed);
			if (i.keyboard[(int)io::KEYBOARD::D] == true)
				camera_move_right(cam, speed);
		}

		//Wheel
		{
			camera_zoom(cam, i.wheel_dir);
		}
	}

	void
	_world_init_testing_scene(ecs::World& w)
	{
		//testing
		{
			//cam
			{
				auto e = world_entity_new(w);
				auto handle_c = world_component_add<world::Camera>(w, e);
				auto data_c = world_handle_component<world::Camera>(w, handle_c);
				*data_c = camera_new();
			}

			//sphere 1
			{
				auto e = world_entity_new(w);
				auto handle_m = world_component_add<world::Mesh>(w, e);
				auto data_m = world_handle_component<world::Mesh>(w, handle_m);
				*data_m = world::mesh_create(DIR_PATH"/res/stls/sphere.stl");

				auto handle_t = world_component_add<world::Transform>(w, e);
				auto data_t = world_handle_component<world::Transform>(w, handle_t);
				*data_t = world::Transform{ 0.0, math::Y_AXIS, math::vec3f{ 1,1,1 }, math::vec3f{ -2,1,-20 } };
			}

			//sphere 2
			{
				auto e = world_entity_new(w);
				auto handle_m = world_component_add<world::Mesh>(w, e);
				auto data_m = world_handle_component<world::Mesh>(w, handle_m);
				*data_m = world::mesh_create(DIR_PATH"/res/stls/sphere.stl");

				auto handle_t = world_component_add<world::Transform>(w, e);
				auto data_t = world_handle_component<world::Transform>(w, handle_t);
				*data_t = world::Transform{ 0.0, math::Y_AXIS, math::vec3f{ 1,1,1 }, math::vec3f{ 2,1,1 } };
			}
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

		//init testing ecs world
		app->ecs_w = ecs::world_new();
		_world_init_testing_scene(app->ecs_w);

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
	_painter_update(Painter app, int window_width, int window_height)
	{
		world::Camera& cam = ecs::world_components_data<world::Camera>(app->ecs_w).front().data;
		__input_act(app->i, cam);
		input_mouse_update(app->i);
		world::camera_viewport(cam, math::vec2f{ (float)window_width, (float)window_height });
	}

	void
	_painter_paint(Painter app, void* palette, unsigned int width, unsigned int height)
	{
		_engine_world_draw(app->e, app->ecs_w, palette);
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
		engine_free(app->e);
		ecs::world_free(app->ecs_w);

		delete app;
	}
};