#include "Engine.h"

#include "world/system/rendering/PBR.h"
#include "world/system/rendering/Phong.h"
#include "world/system/rendering/Colored.h"
#include "world/system/rendering/Point.h"
#include "world/system/rendering/Line.h"
#include "world/system/rendering/Skybox.h"

#include "math/Vector.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl3.h"

#include "gl/glgpu.h"
#include "gl/gl_context.h"

#include "defs/Defs.h"

using namespace world;

namespace rndr
{
	struct IEngine
	{
		//glContext
		glgpu::Context ctx;

		//rendering mode
		Rendering style;

		//rendering systems
		system::PBR_System pbr;
		system::Phong_System phong;
		system::Colored_System colored;
		system::Point_System point;
		system::Line_System line;
		system::Skybox_System skybox;
	};

	//API
	Engine
	engine_create()
	{
		IEngine* self = new IEngine;

		//glcontext
		self->ctx = glgpu::context_create(4, 0);

		//init rendering style
		self->style = Rendering::PBR;

		//rendering systems, pbr_new crashes renderdoc -- revisit
		self->pbr = system::pbr_new();
		self->phong = system::phong_new();
		self->colored = system::colored_new();
		self->point = system::point_new();
		self->line = system::line_new();
		self->skybox = system::skybox_hdr_new(DIR_PATH"/res/imgs/hdr/Tokyo_spec.hdr");

		//imgui
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init("#version 400");

		return self;
	}

	void
	engine_free(Engine e)
	{
		system::pbr_free(e->pbr);
		system::phong_free(e->phong);
		system::colored_free(e->colored);
		system::point_free(e->point);
		system::line_free(e->line);
		system::skybox_free(e->skybox);

		glgpu::context_free(e->ctx);
		delete e;
	}

	void
	engine_rendering_style(Engine e, Rendering mode)
	{
		e->style = mode;
	}

	void
	engine_world_draw(Engine e, ecs::World& w, void* win)
	{
		//attach current glcontext to palette, make sure that palette handle got the default pixel format first
		glgpu::context_attach(e->ctx, win);

		//render scene
		{
			glgpu::frame_start();
			switch (e->style)
			{
				case Rendering::PHONG:
					world::system::phong_run(e->phong, w);
					break;
				case Rendering::PBR:
					world::system::pbr_run(e->pbr, w);
					break;
				case Rendering::COLORED:
					world::system::colored_run(e->colored, w);
					break;
				default:
					break;
			}
			world::system::point_run(e->point, w);
			world::system::line_run(e->line, w);
			world::system::skybox_run(e->skybox, w);
		}
	}

	void
	engine_imgui_draw(Engine e, math::vec2f mouse_pos, bool mouse[3], void* win, unsigned int width, unsigned int height)
	{
		//imgui newframes
		ImGui_ImplWin32_Init(win);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		//set io state
		ImGuiIO& imgui_io = ImGui::GetIO();
		imgui_io.DisplaySize.x = width;
		imgui_io.DisplaySize.y = height;
		imgui_io.MousePos.x = mouse_pos[0];
		imgui_io.MousePos.y = mouse_pos[1];
		imgui_io.MouseClicked[0] = mouse[0];
		imgui_io.MouseClicked[1] = mouse[1];
		imgui_io.MouseClicked[2] = mouse[2];
		//push to imgui cmds
		ImGui::ShowDemoWindow();

		//render gui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
};