#pragma once

#include <backend/backend.h>

#include <IO/Input.h>

#include <math/Vector.h>

#include "window/window.h"
#include "gpu_gl/gl_context.h"

#include "world/World.h"
#include "engine/Engine.h"

namespace app
{
	struct application : backend::callbacks
	{
		//win and ogl context
		win::Window win;
		glgpu::Context ctx;

		//win res
		math::vec2f window_size;

		//input state
		io::Input i;

		//world and rendering engine
		rndr::Engine e;
		world::World* w;
		
		application(int argc, char** argv);

		~application();

		void
		run();

		void
		update() override;

		void
		mouse_handle(int x, int y) override;

		void
		mouse_wheel_handle(int a, int dir, int x, int y) override;

		void
		keyboard_press_handle(unsigned char c, int x, int y) override;

		void
		keyboard_release_handle(unsigned char c, int x, int y) override;

		void
		window_resize_handle(int width, int height) override;
	};
};