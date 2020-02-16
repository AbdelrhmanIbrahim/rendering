#pragma once

#include "window/Window.h"

namespace glgpu
{
	typedef struct IContext* Context;

	//you can't create an opengl context without a window handle/device context, because windows manager system I guess 
	Context
	context_attach(unsigned int gl_major, unsigned int gl_minor, win::Window win);

	void
	context_free(Context self);
};