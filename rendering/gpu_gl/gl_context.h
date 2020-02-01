#pragma once

#include "window/window.h"

namespace glgpu
{
	typedef struct IContext* Context;

	Context
	context_new(unsigned int gl_major, unsigned int gl_minor, win::Window win);

	void
	context_free(Context self);

	//return HGLRC handle of the context (make it better later)
	void*
	context_hgl(Context self);
};