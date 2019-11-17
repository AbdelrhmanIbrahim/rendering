#pragma once

#include "backend/backend.h"

#include <IO/Input.h>

namespace world
{
	struct World;
}

namespace rndr
{
	struct Engine;
}

namespace app
{
	struct application : backend::callbacks
	{
		io::Input i;
		world::World* w;
		rndr::Engine* e;

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
	};
};