#pragma once

#include <backend/backend.h>

#include "IO/Input.h"

#include "math/Vector.h"

#include "window/window.h"
#include "gpu_gl/gl_context.h"

#include "world/World.h"
#include "engine/Engine.h"

namespace app
{
	struct application : backend::callbacks
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

		application(int argc, char** argv);

		~application();

		void
		run();

		void
		update() override;

		void
		mouse_wheel_handle(int a, int dir, int x, int y) override;
	};
};