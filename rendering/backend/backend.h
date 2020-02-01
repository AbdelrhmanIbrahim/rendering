#pragma once

namespace backend
{
	struct callbacks
	{
		virtual void
		update() = 0;

		virtual void
		mouse_wheel_handle(int a, int dir, int x, int y) = 0;
	};

	static callbacks* cb;
	void
	update();

	void
	mouse_wheel_handle(int a, int dir, int x, int y);

	int
	ticks();

	void
	callbacks_init(int argc, char** argv);

	void
	callbacks_set(callbacks* cb);

	void
	callbacks_run();

	void
	callbacks_update();
};