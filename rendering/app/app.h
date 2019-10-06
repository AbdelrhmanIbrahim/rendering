#pragma once

#include "backend/backend.h"
#include "world/World.h"
#include "engine/Engine.h"

namespace app
{
	struct application : backend::callbacks
	{
		world::World w;
		rndr::Engine e;

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