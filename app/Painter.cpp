#include "app/Painter.h"

#include "defs/Defs.h"

#include "io/Input.h"
#include "io/Event.h"

#include "math/Vector.h"

#include "engine/Engine.h"

#include "ecs/World.h"
#include "world/component/Transform.h"
#include "world/component/Mesh.h"
#include "world/component/Camera.h"
#include "world/component/Material.h"
#include "world/component/Flash.h"

using namespace world;
using namespace rndr;
using namespace ecs;
using namespace io;

namespace app
{
	struct IPainter
	{
		//input state
		io::Input input;

		//rendering engine and world
		rndr::Engine engine;
		ecs::World world;

		//rendering mode
		Rendering style;
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

	void
	_world_init_testing_scene(ecs::World& w)
	{
		//entities
		{
			//cam
			{
				auto e = world_entity_new(w);
				auto handle_c = world_component_add<world::Camera>(w, e);
				auto data_c = world_handle_component<world::Camera>(w, handle_c);
				*data_c = camera_new();

				auto handle_f = world_component_add<world::Flash>(w, e);
				auto data_f = world_handle_component<world::Flash>(w, handle_f);
				*data_f = world::Flash{ {1.0f, 1.0f, 1.0f, 1.0f},
										{0.0f, 0.0f, 0.0f, 0.0f },
										{0, 0, -1, 0}, 10,
										(float)cos(to_radian(12)),
										(float)cos(to_radian(15)) };
			}

			//sphere 1
			{
				auto e = world_entity_new(w);
				auto handle_m = world_component_add<world::Mesh>(w, e);
				auto data_m = world_handle_component<world::Mesh>(w, handle_m);
				*data_m = world::mesh_create(DIR_PATH"/res/stls/sphere.stl");

				auto handle_t = world_component_add<world::Transform>(w, e);
				auto data_t = world_handle_component<world::Transform>(w, handle_t);
				*data_t = world::Transform{ 0.0, math::Y_AXIS, math::vec3f{ 1,1,1 }, math::vec3f{ 0, 0, -6} };

				auto handle_c = world_component_add<world::Material>(w, e);
				auto data_c = world_handle_component<world::Material>(w, handle_t);
				*data_c = world::Material{math::vec4f{ 0.75, 0.75, 0.75, 1.0f  }, 0.9, 0.2 };
			}

			//sphere 2
			{
				auto e = world_entity_new(w);
				auto handle_m = world_component_add<world::Mesh>(w, e);
				auto data_m = world_handle_component<world::Mesh>(w, handle_m);
				*data_m = world::mesh_create(DIR_PATH"/res/stls/sphere.stl");

				auto handle_t = world_component_add<world::Transform>(w, e);
				auto data_t = world_handle_component<world::Transform>(w, handle_t);
				*data_t = world::Transform{ 0.0, math::Y_AXIS, math::vec3f{ 1,1,1 }, math::vec3f{ 1, 0, 2 } };

				auto handle_c = world_component_add<world::Material>(w, e);
				auto data_c = world_handle_component<world::Material>(w, handle_t);
				*data_c = world::Material{ math::vec4f{  0.75, 0.75, 0.75, 1.0f  }, 0.9, 0.2 };
			}
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

		//init testing ecs world
		app->world = ecs::world_new();
		_world_init_testing_scene(app->world);

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
		//create entity
		auto stl = world_entity_new(app->world);

		//add mesh comp
		auto handle_m = world_component_add<world::Mesh>(app->world, stl);
		auto data_m = world_handle_component<world::Mesh>(app->world, handle_m);
		*data_m = world::mesh_create(path);
		if (data_m->vertices.empty())
			return false;

		//add transform component
		auto handle_t = world_component_add<world::Transform>(app->world, stl);
		auto data_t = world_handle_component<world::Transform>(app->world, handle_t);
		*data_t = world::Transform{ 0.0, math::Y_AXIS, math::vec3f{ 1,1,1 }, math::vec3f{0,0,-1}};

		return true;
	}

	void
	painter_input(Painter app, Event event)
	{
		input_process_event(app->input, event);
	}

	void
	painter_update(Painter app, int window_width, int window_height)
	{
		auto& cam = ecs::world_components_data<world::Camera>(app->world).front();
		auto handle = ecs::world_component_add<world::Flash>(app->world, cam.entity);
		auto flash = ecs::world_handle_component<world::Flash>(app->world, handle);

		_input_act(app->input, cam.data, *flash);
		input_mouse_update(app->input);
		world::camera_viewport(cam.data, math::vec2f{ (float)window_width, (float)window_height });
	}

	void
	painter_paint(Painter app, void* palette, unsigned int width, unsigned int height)
	{
		engine_world_draw(app->engine, app->world, palette);
		engine_imgui_draw(app->engine, math::vec2f{ (float)app->input.mouse_x, (float)app->input.mouse_y}, app->input.mouse, palette, width, height);
		win::window_swap(palette);
	}

	void
	painter_free(Painter app)
	{
		engine_free(app->engine);
		ecs::world_free(app->world);

		delete app;
	}
};