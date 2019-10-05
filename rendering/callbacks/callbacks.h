#pragma once

namespace cbs
{
	struct callbacks
	{
		virtual void
		update() = 0;

		virtual void
		mouse_handle(int x, int y) = 0;

		virtual void
		keyboard_handle(unsigned char c, int x, int y) = 0;
	};

	static callbacks* cb;
	void
	update();

	void
	mouse_handle(int x, int y);

	void
	keyboard_handle(unsigned char c, int x, int y);


	//init callbacks backend
	void
	callbacks_init(int argc, char** argv);

	void
	callbacks_set(callbacks* cb);

	void
	callbacks_run();
};