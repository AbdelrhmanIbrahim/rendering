#include "app/App.h"

#include "defs/Defs.h"

#include "io/Input.h"

#include "math/Vector.h"

#include "window/Window.h"
#include "gl/gl_context.h"

#include "world/World.h"
#include "engine/Engine.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl3.h"

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

	void
	_imgui_render(const io::Input& app_i, math::vec2f& win_size)
	{
		//imgui newframes
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		//set io state
		ImGuiIO& imgui_io = ImGui::GetIO();
		imgui_io.DisplaySize.x = win_size[0];
		imgui_io.DisplaySize.y = win_size[1];
		imgui_io.MousePos.x = app_i.mouse_x;
		imgui_io.MousePos.y = app_i.mouse_y;
		imgui_io.MouseClicked[0] = app_i.mouse[0];
		imgui_io.MouseClicked[1] = app_i.mouse[1];
		imgui_io.MouseClicked[2] = app_i.mouse[2];

		//push to imgui cmds
		ImGui::ShowDemoWindow();

		//render gui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	//API
	App
	app_new()
	{
		IApp* app = new IApp;

		//window and its context
		app->window_size = math::vec2f{ WIN_WIDTH, WIN_HEIGHT };
		app->win = win::window_new(WIN_WIDTH, WIN_HEIGHT, "rendering journey");
		app->ctx = glgpu::context_create(4, 0, app->win);

		//input init state
		app->i = Input{};
		app->i.mouse_x = WIN_WIDTH / 2;
		app->i.mouse_y = WIN_HEIGHT / 2;
		app->i.pmouse_x = WIN_WIDTH / 2;
		app->i.pmouse_y = WIN_HEIGHT / 2;

		//init rendering engine and world
		app->e = engine_create();
		app->w = world_create();

		app->is_running = true;

		//init imgui
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(win::window_handle(app->win));
		ImGui_ImplOpenGL3_Init("#version 400");

		return app;
	}

	void
	app_input(App app, win::Window_Event event)
	{
		if (event.kind == win::Window_Event::KIND::KIND_WINDOW_RESIZE)
		{
			app->window_size[0] = event.window_resize.width;
			app->window_size[1] = event.window_resize.height;
		}
		else if (event.kind == win::Window_Event::KIND::KIND_WINDOW_CLOSE)
			app->is_running = false;

		input_process_event(app->i, event);
	}

	void
	app_update(App app)
	{
		_input_act(app->i, app->w);
		input_mouse_update(app->i);
	}

	void
	app_paint(App app, win::Window palette)
	{
		//attach context
		glgpu::context_attach(app->ctx, palette);
		{
			//render world
			camera_viewport(app->w->cam, app->window_size);
			engine_world_draw(app->e, app->w);

			//render GUI
			_imgui_render(app->i, app->window_size);
		}
		window_swap(palette);
	}

	bool
	app_running(App app)
	{
		return app->is_running;
	}

	win::Window
	app_window(App app)
	{
		return app->win;
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