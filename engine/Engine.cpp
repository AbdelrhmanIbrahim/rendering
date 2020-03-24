#include "Engine.h"

#include "world/World.h"
#include "world/Camera.h"

#include "engine/renderers/Phong.h"
#include "engine/renderers/PBR.h"
#include "engine/renderers/Skybox.h"
#include "engine/renderers/Phong_Shadow.h"


#include "math/Vector.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl3.h"

//(refactor later --revisit--)
#include "gl/glgpu.h"
#include "gl/gl_context.h"

using namespace world;

namespace rndr
{
	struct IEngine
	{
		//glContext
		glgpu::Context ctx;

		//renderers
		Phong phong;
		PBR pbr;
		Skybox skybox;
		Phong_Shadow phong_shadow;
	};

	Engine
	engine_create()
	{
		IEngine* self = new IEngine;

		//glcontext
		self->ctx = glgpu::context_create(4, 0);

		//imgui
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init("#version 400");

		//renderers
		self->phong = phong_create();
		//self->pbr = pbr_create();
		//self->skybox = skybox_renderer_hdr_create(DIR_PATH"/res/imgs/hdr/Tokyo_spec.hdr");
		//self->phong_shadow = phong_shadow_create();

		//skybox
		/*static const char* skybox_paths[6]
		{
			"../res/imgs/skybox/sky/right.jpg",
			"../res/imgs/skybox/sky/left.jpg",
			"../res/imgs/skybox/sky/bottom.jpg",
			"../res/imgs/skybox/sky/top.jpg",
			"../res/imgs/skybox/sky/front.jpg",
			"../res/imgs/skybox/sky/back.jpg"
		};
		self->skybox = skybox_renderer_rgba_create(skybox_paths, io::IMAGE_FORMAT::JPG);*/

		return self;
	}

	void
	engine_free(Engine e)
	{
		phong_free(e->phong);
		//pbr_free(e->pbr);
		//skybox_renderer_free(e->skybox);
		//phong_shadow_free(e->phong_shadow);

		glgpu::context_free(e->ctx);

		delete e;
	}

	void
	engine_world_draw(Engine e, const World* w, void* win)
	{
		//attach current glcontext to palette, make sure that palette handle got the default pixel format first
		glgpu::context_attach(e->ctx, win);

		//render scene
		{
			//enable tests and clear color and depth buffers (refactor later)
			glgpu::frame_start();

			//pack meshes to draw
			for (const auto& mesh : w->meshes)
			{
				phong_pack(e->phong, &mesh);
				//pbr_pack(e->pbr, &mesh);
				//phong_shadow_pack(e->phong_shadow, &mesh);
			}

			//flush renderers
			{
				phong_draw(e->phong, w->cam);
				//pbr_draw(e->pbr, w->cam);
				//phong_shadow_draw(e->phong_shadow, math::vec3f{0,30,0}, w->cam);
			}

			//unpack meshes
			{
				phong_unpack(e->phong);
				//pbr_unpack(e->pbr);
				//phong_shadow_unpack(e->phong_shadow);
			}

			//skybox
			{
				//skybox_renderer_draw(e->skybox, w->cam);
			}
		}

	}

	void
	engine_imgui_draw(Engine e, const io::Input& app_i, void* win, unsigned int width, unsigned int height)
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
};