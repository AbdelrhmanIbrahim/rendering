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
	keyboard_press(unsigned char c, int x, int y)
	{
		cb->keyboard_press_handle(c, x, y);
	}

	void
	keyboard_release(unsigned char c, int x, int y)
	{
		cb->keyboard_release_handle(c, x, y);
	}

	void
	mouse_wheel_handle(int a, int b,int x, int y)
	{
		cb->mouse_wheel_handle(a, b, x, y);
	}

	void
	window_resize_handle(int width, int height)
	{
		cb->window_resize_handle(width, height);
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
		glutKeyboardFunc(keyboard_press);
		glutKeyboardUpFunc(keyboard_release);
		glutPassiveMotionFunc(mouse_handle);
		glutMouseWheelFunc(mouse_wheel_handle);
		glutReshapeFunc(window_resize_handle);
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