#include "app/App.h"

#include "Defs/Defs.h"

#include "IO/Input.h"

#include "math/Vector.h"

#include "window/Window.h"
#include "GL/gl_context.h"

#include "world/World.h"
#include "engine/Engine.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_opengl3.h"


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

		//init rendering engine and world
		app->e = engine_create();
		app->w = world_create();
		
		//init imgui
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(win::window_handle(app->win));
		ImGui_ImplOpenGL3_Init("#version 400");

		return app;
	}

	void
	app_run(App app)
	{
		while (true)
		{
			//INPUT
			{
				win::Window_Event event = win::window_poll(app->win);
				if (event.kind == win::Window_Event::KIND::KIND_WINDOW_RESIZE)
				{
					app->window_size[0] = event.window_resize.width;
					app->window_size[1] = event.window_resize.height;
				}
				else if (event.kind == win::Window_Event::KIND::KIND_WINDOW_CLOSE)
					break;

				input_process_event(app->i, event);
			}

			//UPDATE
			{
				_input_act(app->i, app->w);
				input_mouse_update(app->i);

			}

			//RENDER
			{
				//render world
				camera_viewport(app->w->cam, app->window_size);
				engine_world_draw(app->e, app->w);

				//render GUI
				_imgui_render(app->i, app->window_size);

				//Swap window buffers
				window_swap(app->win);
			}

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