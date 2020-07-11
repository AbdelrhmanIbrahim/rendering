#pragma once

namespace glgpu
{
	typedef struct IContext* Context;

	Context
	context_create(unsigned int gl_major, unsigned int gl_minor);

	void
	context_attach(Context self, void* win);

	void
	context_free(Context self);
};