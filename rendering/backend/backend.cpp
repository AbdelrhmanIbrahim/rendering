#include "backend.h"

#include <freeglut.h>
#include <assert.h>

namespace backend
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
	mouse_wheel_handle(int a, int b,int x, int y)
	{
		cb->mouse_wheel_handle(a, b, x, y);
	}

	int
	ticks()
	{
		return glutGet(GLUT_ELAPSED_TIME);
	}

	void
	callbacks_init(int argc, char** argv)
	{
		glutInit(&argc, argv);
	}

	void
	callbacks_set(callbacks* cb)
	{
		backend::cb = cb;
		glutDisplayFunc(update);
		glutPassiveMotionFunc(mouse_handle);
		glutKeyboardFunc(keyboard_handle);
		glutMouseWheelFunc(mouse_wheel_handle);
		glutIdleFunc(update);
	}

	void
	callbacks_run()
	{
		glutMainLoop();
	}

	void
	callbacks_update()
	{
		glutSwapBuffers();
	}
};