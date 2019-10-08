#pragma once

#include "backend/backend.h"

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
		keyboard_handle(unsigned char c, int x, int y) override;
	};
};