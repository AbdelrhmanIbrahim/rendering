#include "backend.h"

#include <glew.h>
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
	graphics_init()
	{
		GLenum gl_ok = glewInit();
		assert(gl_ok == GLEW_OK);
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