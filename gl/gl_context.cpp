//winos
#ifdef OS_WINDOWS
	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <windowsx.h>
#endif

#include "gl_context.h"

#include <assert.h>

#include "glew.h"
#include "wglew.h"

#include "utils/Defer.h"

namespace glgpu
{
	struct IContext
	{
		HGLRC context;
	};

	LRESULT CALLBACK
	_fake_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//postquitmessage teminates the current thread, so that's why when creating gl_context from here inside a qt app, it crashes as the thread ends
		return DefWindowProcA(hwnd, msg, wparam, lparam);
	}

	Context
	context_create(unsigned int gl_major, unsigned int gl_minor)
	{
		IContext* self = new IContext;
		
		win::Window fake_win = win::window_fake_new(1, 1, "glew init fake window");
		HDC fake_dc = (HDC)win::window_dc(fake_win);
		defer(fake_free, win::window_free(fake_win));

		HGLRC fake_ctx = wglCreateContext(fake_dc);
		assert(fake_ctx && "wglCreateContext failed");

		bool result = wglMakeCurrent(fake_dc, fake_ctx);
		assert(result && "wglMakeCurrent failed");

		//GLEW initialized
		GLenum glew_result = glewInit();
		assert(glew_result == GLEW_OK && "glewInit failed");

		win::Window ctx_dummy = win::window_fake_new(1, 1, "glcontext init dummy window");
		HDC dc = (HDC)win::window_dc(ctx_dummy);
		defer(dummy_free, win::window_free(ctx_dummy));

		//now we are in a position to create the modern opengl context
		int context_attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, gl_major,
			WGL_CONTEXT_MINOR_VERSION_ARB, gl_minor,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		self->context = wglCreateContextAttribsARB(dc, 0, context_attribs);
		assert(self->context && "wglCreateContextAttribsARB failed");

		result = wglMakeCurrent(dc, self->context);
		assert(result && "wglMakeCurrent failed");

		result = wglSwapIntervalEXT(0);
		assert(result && "disabling vsync failed");

		result = wglDeleteContext(fake_ctx);
		assert(result && "wglDeleteContext failed");

		return self;
	}

	void
	context_attach(Context self, void* win)
	{
		HDC dc = GetDC((HWND)win);
		bool result = wglMakeCurrent(dc, self->context);
		assert(result && "wglMakeCurrent failed");

		//replace somewhere else in the init
		result = wglSwapIntervalEXT(0);
		assert(result && "disabling vsync failed");
	}

	void 
	context_free(Context self)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(self->context);
		delete self;
	}
};