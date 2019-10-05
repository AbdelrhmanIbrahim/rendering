#pragma once

#include "callbacks/callbacks.h"

namespace app
{
	struct application : cbs::callbacks
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