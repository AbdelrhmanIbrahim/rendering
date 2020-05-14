#include "Engine.h"

#include "math/Vector.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl3.h"

#include "gl/glgpu.h"
#include "gl/gl_context.h"

#include "defs/Defs.h"

#include "engine/renderer/Phong.h"
#include "engine/renderer/PBR.h"
#include "engine/renderer/Point.h"
#include "engine/renderer/Line.h"
#include "engine/renderer/Edge.h"
#include "engine/renderer/Quad.h"
#include "engine/renderer/Hiddenline.h"
#include "engine/renderer/Skybox.h"

#include "world/system/rendering/Phong.h"
#include "world/system/rendering/PBR.h"
#include "world/system/rendering/Colored.h"
#include "world/system/rendering/Point.h"
#include "world/system/rendering/Line.h"
#include "world/system/rendering/Background.h"
#include "world/system/rendering/Hiddenline.h"
#include "world/system/rendering/Skybox.h"

using namespace world;

namespace rndr
{
	struct IEngine
	{
		//glContext
		glgpu::Context ctx;

		//rendering mode
		Rendering style;

		//renderers
		rndr::Phong phong;
		rndr::PBR pbr;
		rndr::Colored colored;
		rndr::Point point;
		rndr::Line line;
		rndr::Edge edge;
		rndr::Cquad cquad;
		rndr::Hiddenline hline;
		rndr::Skybox skybox;
	};

	//helpers
	void
	_engine_world_mesh_render(Engine e, ecs::World& w)
	{
		switch (e->style)
		{
		case Rendering::PHONG:
			world::system::phong_sys_run(e->phong, w);
			break;
		case Rendering::PBR:
		{
			world::system::pbr_sys_run(e->pbr, w);
			world::system::skybox_sys_run(e->skybox, w);
			break;
		}
		case Rendering::HIDDENLINE:
			world::system::hiddenline_sys_run(e->hline, w);
			break;
		case Rendering::COLORED:
			world::system::colored_sys_run(e->colored, w);
			break;
		default:
			break;
		}
	}

	//API
	Engine
	engine_create()
	{
		IEngine* self = new IEngine;

		//glcontext
		self->ctx = glgpu::context_create(4, 0);

		//init rendering style
		self->style = Rendering::PHONG;

		//allocate renderers, pbr_create crashes renderdoc -- revisit
		self->phong = rndr::phong_create();
		self->pbr = rndr::pbr_create();
		self->colored = rndr::colored_create();
		self->point = rndr::point_create();
		self->line = rndr::line_create();
		self->edge = rndr::edge_create();
		self->cquad = rndr::cquad_create();
		self->hline = rndr::hiddenline_create();
		self->skybox = rndr::skybox_renderer_hdr_create(DIR_PATH"/res/imgs/hdr/Tokyo_spec.hdr");

		//imgui
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init("#version 400");

		return self;
	}

	void
	engine_free(Engine e)
	{
		rndr::phong_free(e->phong);
		rndr::pbr_free(e->pbr);
		rndr::colored_free(e->colored);
		rndr::point_free(e->point);
		rndr::line_free(e->line);
		rndr::edge_free(e->edge);
		rndr::cquad_free(e->cquad);
		rndr::hiddenline_free(e->hline);
		rndr::skybox_renderer_free(e->skybox);

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

		//run rendering systems
		{
			glgpu::frame_start(0.6f, 0.68f, 0.5f);
			_engine_world_mesh_render(e, w);
			world::system::point_sys_run(e->point, w);
			world::system::line_sys_run(e->line, w);
			world::system::bg_sys_run(e->cquad, w);
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

	rndr::Colored
	engine_colored_renderer(Engine e)
	{
		return e->colored;
	}
};