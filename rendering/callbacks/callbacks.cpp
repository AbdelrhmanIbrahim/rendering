#include "callbacks.h"

#include <freeglut.h>

namespace cbs
{
	void
	update()
	{
		cb->update();
	}

	void
	mouse_handle(int x, int y)
	{
		cb->mouse_handle(x, y);
	}

	void
	keyboard_handle(unsigned char c, int x, int y)
	{
		cb->keyboard_handle(c, x, y);
	}


	void
	callbacks_init(int argc, char** argv)
	{
		glutInit(&argc, argv);
	}

	void
	callbacks_set(callbacks* cb)
	{
		cbs::cb = cb;
		glutDisplayFunc(update);
		glutPassiveMotionFunc(mouse_handle);
		glutKeyboardFunc(keyboard_handle);
		//glutIdleFunc(update);
	}

	void
	callbacks_run()
	{
		glutMainLoop();
	}
};