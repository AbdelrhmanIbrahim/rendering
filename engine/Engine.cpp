#include "Engine.h"

#include "world/component/Camera.h"
#include "world/component/Mesh.h"
#include "world/component/Transform.h"
#include "world/component/Material.h"
#include "world/system/rendering/Phong.h"

#include "engine/renderer/Phong.h"
#include "engine/renderer/PBR.h"
#include "engine/renderer/Skybox.h"
#include "engine/renderer/Phong_Shadow.h"
#include "engine/renderer/Colored.h"

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
		Colored colored;
		//Phong_Shadow phong_shadow;
	};

	//Internal helpers
	void
	_pbr_render(PBR pbr, const Camera& cam,
						 const std::vector<ecs::Component<Mesh>>& meshes,
					 	 const std::vector<ecs::Component<Transform>>& transforms,
						 const std::vector<ecs::Component<Material>>& materials)
	{
		pbr_set(pbr, &cam);
		for (int i = 0; i < meshes.size(); ++i)
		{
			if (meshes[i].deleted == false)
				pbr_draw(pbr, camera_view_proj(cam), &meshes[i].data, &transforms[i].data, &materials[i].data);
		}
	}

	void
	_colored_render(Colored colored, const Camera& cam,
			const std::vector<ecs::Component<Mesh>>& meshes,
			const std::vector<ecs::Component<Transform>>& transforms,
			const std::vector<ecs::Component<Material>>& materials)
	{
		math::vec2f viewport = world::camera_viewport(cam);
		colored_set(colored, viewport);
		for (int i = 0; i < meshes.size(); ++i)
		{
			if (meshes[i].deleted == false)
				colored_draw(colored, camera_view_proj(cam), &meshes[i].data, &transforms[i].data, materials[i].data.color_norm);
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

		//crashes renderdoc -- revisit
		self->pbr = pbr_create();

		self->skybox = skybox_renderer_hdr_create(DIR_PATH"/res/imgs/hdr/Tokyo_spec.hdr");
		self->colored = colored_create();
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
		colored_free(e->colored);
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

		//render scene
		{
			//start the frame
			glgpu::frame_start();

			//get data for drawing
			auto& cam = ecs::world_components_data<world::Camera>(w).front().data;
			auto& meshes = ecs::world_components_data<world::Mesh>(w);
			auto& transforms = ecs::world_components_data<world::Transform>(w);
			auto& materials = ecs::world_components_data<world::Material>(w);

			switch (e->style)
			{
				case Rendering::PHONG:
				{
					world::system::phong_render(e->phong, w);
					break;
				}
				case Rendering::PBR:
					_pbr_render(e->pbr, cam, meshes, transforms, materials);
					break;
				case Rendering::COLORED:
					_colored_render(e->colored, cam, meshes, transforms, materials);
					break;
				default:
					break;
			}
			skybox_renderer_draw(e->skybox, &cam);
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