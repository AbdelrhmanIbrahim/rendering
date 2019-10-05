#include "window.h"

#include <freeglut.h>

namespace win
{
	void
	window_create(unsigned int width, unsigned int height, const char* name)
	{
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		glutInitWindowPosition(0, 0);
		glutInitWindowSize(width, height);
		glutCreateWindow(name);
	}
};