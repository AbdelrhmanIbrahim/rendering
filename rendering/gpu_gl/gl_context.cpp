//winos
#ifdef WINOS
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#endif

#include "gl_context.h"

#include <assert.h>

#include "glew.h"
#include "wglew.h"

namespace glgpu
{
	struct IContext
	{
		HGLRC context;
	};

	LRESULT CALLBACK
	_fake_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		default:
			break;
		}

		return DefWindowProcA(hwnd, msg, wparam, lparam);
	}

	Context
	context_attach(unsigned int gl_major, unsigned int gl_minor, win::Window win)
	{
		IContext* self = new IContext;

		//Setup Window Class that we'll use down there
		WNDCLASSEXA wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEXA));
		wc.cbSize = sizeof(WNDCLASSEXA);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = _fake_window_proc;
		wc.hInstance = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.lpszClassName = "hiddenWindowClass";

		RegisterClassExA(&wc);

		//1 pixel window dimension since all the windows we'll be doing down there are hidden
		RECT wr = { 0, 0, LONG(1), LONG(1) };
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

		//The first step in creating Modern GL Context is to create Legacy GL Context
		//so we setup fake window and dc to create the legacy context so that we could
		//initialize GLEW which will use wglGetProcAddress to load Modern OpenGL implmentation
		//off the GPU driver
		HWND fake_wnd = CreateWindowExA(NULL,
			"hiddenWindowClass",
			"Fake Window",
			WS_OVERLAPPEDWINDOW,
			100,
			100,
			wr.right - wr.left,
			wr.bottom - wr.top,
			NULL,
			NULL,
			NULL,
			NULL);

		HDC fake_dc = GetDC(fake_wnd);

		PIXELFORMATDESCRIPTOR fake_pfd;
		ZeroMemory(&fake_pfd, sizeof(fake_pfd));
		fake_pfd.nSize = sizeof(fake_pfd);
		fake_pfd.nVersion = 1;
		fake_pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE;
		fake_pfd.iPixelType = PFD_TYPE_RGBA;
		fake_pfd.cColorBits = 32;
		fake_pfd.cAlphaBits = 8;
		fake_pfd.cDepthBits = 24;
		fake_pfd.iLayerType = PFD_MAIN_PLANE;

		int fake_pfdid = ChoosePixelFormat(fake_dc, &fake_pfd);
		assert(fake_pfdid && "ChoosePixelFormat failed");

		bool result = SetPixelFormat(fake_dc, fake_pfdid, &fake_pfd);
		assert(result && "SetPixelFormat failed");

		HGLRC fake_ctx = wglCreateContext(fake_dc);
		assert(fake_ctx && "wglCreateContext failed");

		result = wglMakeCurrent(fake_dc, fake_ctx);
		assert(result && "wglMakeCurrent failed");

		//At last GLEW initialized
		GLenum glew_result = glewInit();
		assert(glew_result == GLEW_OK && "glewInit failed");

		//setup the modern pixel format in order to create the modern GL Context
		const int pixel_attribs[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_ALPHA_BITS_ARB, 8,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
			WGL_SAMPLES_ARB, 2,
			0, 0
		};

		HDC dc = (HDC)win::window_dc(win);
		int pixel_format_id;
		UINT num_formats;
		bool status = wglChoosePixelFormatARB(dc,
			pixel_attribs,
			NULL,
			1,
			&pixel_format_id,
			&num_formats);

		assert(status && num_formats > 0 && "wglChoosePixelFormatARB failed");

		PIXELFORMATDESCRIPTOR pixel_format{};
		DescribePixelFormat(dc, pixel_format_id, sizeof(pixel_format), &pixel_format);
		SetPixelFormat(dc, pixel_format_id, &pixel_format);

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

		{
			glEnable(GL_DEPTH_TEST);
			glClearColor(0, 1, 0, 1);
		}

		result = wglDeleteContext(fake_ctx);
		assert(result && "wglDeleteContext failed");

		result = ReleaseDC(fake_wnd, fake_dc);
		assert(result && "ReleaseDC failed");

		result = DestroyWindow(fake_wnd);
		assert(result && "DestroyWindow failed");

		return self;
	}

	void 
	context_free(Context self)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(self->context);
		delete self;
	}
};