#pragma once

namespace backend
{
	struct callbacks
	{
		virtual void
		update() = 0;

		virtual void
		mouse_handle(int x, int y) = 0;

		virtual void
		keyboard_press_handle(unsigned char c, int x, int y) = 0;

		virtual void
		keyboard_release_handle(unsigned char c, int x, int y) = 0;

		virtual void
		mouse_wheel_handle(int a, int dir, int x, int y) = 0;

		virtual void
		window_resize_handle(int width, int height) = 0;
	};

	static callbacks* cb;
	void
	update();

	void
	mouse_handle(int x, int y);

	void
	keyboard_press(unsigned char c, int x, int y);

	void
	keyboard_release(unsigned char c, int x, int y);

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