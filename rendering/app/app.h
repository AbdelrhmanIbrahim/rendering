#pragma once

#include "backend/backend.h"

namespace app
{
	struct application : backend::callbacks
	{
		application(int argc, char** argv);

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