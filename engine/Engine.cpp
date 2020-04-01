#include "Engine.h"

#include "world/components/Camera.h"
#include "world/components/Mesh.h"
#include "world/components/Transform.h"

#include "engine/renderers/Phong.h"
#include "engine/renderers/PBR.h"
#include "engine/renderers/Skybox.h"
#include "engine/renderers/Phong_Shadow.h"

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

		//renderers
		Phong phong;
		PBR pbr;
		Skybox skybox;
		//Phong_Shadow phong_shadow;
	};

	//Internal helpers
	void
	_pbr_draw(PBR pbr, const Camera& cam, const std::vector<ecs::Component<Mesh>>& meshes, const std::vector<ecs::Component<Transform>>& transforms)
	{
		for (int i = 0; i < meshes.size(); ++i)
		{
			if (meshes[i].deleted == false)
				pbr_draw(pbr, cam, meshes[i].data, transforms[i].data);
		}
	}

	void
	_phong_draw(Phong phong, const Camera& cam, const std::vector<ecs::Component<Mesh>>& meshes, const std::vector<ecs::Component<Transform>>& transforms)
	{
		for (int i = 0; i < meshes.size(); ++i)
		{
			if (meshes[i].deleted == false)
				phong_draw(phong, cam, meshes[i].data, transforms[i].data);
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
		self->style = Rendering::PBR;

		//renderers
		self->phong = phong_create();
		self->pbr = pbr_create();
		self->skybox = skybox_renderer_hdr_create(DIR_PATH"/res/imgs/hdr/Tokyo_spec.hdr");
		//self->phong_shadow = phong_shadow_create();

		//imgui
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init("#version 400");

		return self;
	}

	void
	engine_free(Engine e)
	{
		phong_free(e->phong);
		pbr_free(e->pbr);
		skybox_renderer_free(e->skybox);
		//phong_shadow_free(e->phong_shadow);

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

		//get data for drawing
		auto& cam = ecs::world_components_data<world::Camera>(w)[0].data;
		auto& meshes = ecs::world_components_data<world::Mesh>(w);
		auto& transforms = ecs::world_components_data<world::Transform>(w);

		//render scene
		{
			//start the frame
			glgpu::frame_start();

			//render according to the style
			switch (e->style)
			{
			case Rendering::PHONG:
				_phong_draw(e->phong, cam, meshes, transforms);
				break;
			case Rendering::PBR:
				_pbr_draw(e->pbr, cam, meshes, transforms);
				break;
			default:
				break;
			}
			skybox_renderer_draw(e->skybox, cam);
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