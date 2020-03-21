//winos
#ifdef OS_WINDOWS
	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <windowsx.h>
#endif

#include "Window.h"

#include <assert.h>

namespace win
{
	struct IWindow
	{
		HWND handle;
		HDC dc;
		int width;
		int height;
		const char* title;
		Window_Event event;
	};

	inline static KEYBOARD
	_map_keyboard_key(WPARAM k)
	{
		switch (k)
		{
			//letters
		case 0x41:		return KEYBOARD::A;
		case 0x42:		return KEYBOARD::B;
		case 0x43:		return KEYBOARD::C;
		case 0x44:		return KEYBOARD::D;
		case 0x45:		return KEYBOARD::E;
		case 0x46:		return KEYBOARD::F;
		case 0x47:		return KEYBOARD::G;
		case 0x48:		return KEYBOARD::H;
		case 0x49:		return KEYBOARD::I;
		case 0x4A:		return KEYBOARD::J;
		case 0x4B:		return KEYBOARD::K;
		case 0x4C:		return KEYBOARD::L;
		case 0x4D:		return KEYBOARD::M;
		case 0x4E:		return KEYBOARD::N;
		case 0x4F:		return KEYBOARD::O;
		case 0x50:		return KEYBOARD::P;
		case 0x51:		return KEYBOARD::Q;
		case 0x52:		return KEYBOARD::R;
		case 0x53:		return KEYBOARD::S;
		case 0x54:		return KEYBOARD::T;
		case 0x55:		return KEYBOARD::U;
		case 0x56:		return KEYBOARD::V;
		case 0x57:		return KEYBOARD::W;
		case 0x58:		return KEYBOARD::X;
		case 0x59:		return KEYBOARD::Y;
		case 0x5A:		return KEYBOARD::Z;

			//numbers
		case 0x30:		return KEYBOARD::NUM_0;
		case 0x31:		return KEYBOARD::NUM_1;
		case 0x32:		return KEYBOARD::NUM_2;
		case 0x33:		return KEYBOARD::NUM_3;
		case 0x34:		return KEYBOARD::NUM_4;
		case 0x35:		return KEYBOARD::NUM_5;
		case 0x36:		return KEYBOARD::NUM_6;
		case 0x37:		return KEYBOARD::NUM_7;
		case 0x38:		return KEYBOARD::NUM_8;
		case 0x39:		return KEYBOARD::NUM_9;

			//misc
		case 0x1B:		return KEYBOARD::ESC;
		case 0xBD:		return KEYBOARD::MINUS;
		case 0xBC:		return KEYBOARD::COMMA;
		case 0xBB:		return KEYBOARD::EQUAL;
		case 0xBE:		return KEYBOARD::PERIOD;
		case 0x20:		return KEYBOARD::SPACE;
		case 0x0D:		return KEYBOARD::ENTER;
		case 0x08:		return KEYBOARD::BACKSPACE;
		case 0x09:		return KEYBOARD::TAB;
		case 0xBF:		return KEYBOARD::FORWARD_SLASH;
		case 0xE2:		return KEYBOARD::BACKSLASH;
		case 0x2E:		return KEYBOARD::DLT;
		case 0xA0:		return KEYBOARD::LEFT_SHIFT;
		case 0xA1:		return KEYBOARD::RIGHT_SHIFT;
		case 0x12:		return KEYBOARD::LEFT_ALT;
		case 0xA2:		return KEYBOARD::LEFT_CTRL;
		case 0xA3:		return KEYBOARD::RIGHT_CTRL;
		case 0x23:		return KEYBOARD::END;
		case 0x24:		return KEYBOARD::HOME;
		case 0x2D:		return KEYBOARD::INSERT;
		case 0x21:		return KEYBOARD::PAGEUP;
		case 0x22:		return KEYBOARD::PAGEDOWN;
		case 0x25:		return KEYBOARD::LEFT;
		case 0x26:		return KEYBOARD::UP;
		case 0x27:		return KEYBOARD::RIGHT;
		case 0x28:		return KEYBOARD::DOWN;
		case 0x70:		return KEYBOARD::F1;
		case 0x71:		return KEYBOARD::F2;
		case 0x72:		return KEYBOARD::F3;
		case 0x73:		return KEYBOARD::F4;
		case 0x74:		return KEYBOARD::F5;
		case 0x75:		return KEYBOARD::F6;
		case 0x76:		return KEYBOARD::F7;
		case 0x77:		return KEYBOARD::F8;
		case 0x78:		return KEYBOARD::F9;
		case 0x79:		return KEYBOARD::F10;
		case 0x7A:		return KEYBOARD::F11;
		case 0x7B:		return KEYBOARD::F12;
		default:		return KEYBOARD::COUNT;
		}
	}

	LRESULT CALLBACK
	_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		IWindow* self = (IWindow*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

		switch (msg)
		{
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			if (self)
			{
				self->event.kind = Window_Event::KIND::KIND_WINDOW_CLOSE;
				self->event.window_close.close = true;
			}
			return 0;

		case WM_KEYDOWN:
		{
			self->event.kind = Window_Event::KIND::KIND_KEYBOARD_KEY;
			self->event.keyboard_key.k = _map_keyboard_key(wparam);
			self->event.keyboard_key.s = KEY_STATE::DOWN;
			break;
		}

		case WM_KEYUP:
		{
			self->event.kind = Window_Event::KIND::KIND_KEYBOARD_KEY;
			self->event.keyboard_key.k = _map_keyboard_key(wparam);
			self->event.keyboard_key.s = KEY_STATE::UP;
			break;
		}

		case WM_LBUTTONDOWN:
		{
			self->event.kind = Window_Event::KIND::KIND_MOUSE_BUTTON;
			self->event.mouse_button.b = MOUSE::LEFT;
			self->event.mouse_button.s = KEY_STATE::DOWN;
			break;
		}

		case WM_LBUTTONUP:
		{
			self->event.kind = Window_Event::KIND::KIND_MOUSE_BUTTON;
			self->event.mouse_button.b = MOUSE::LEFT;
			self->event.mouse_button.s = KEY_STATE::UP;
			break;
		}

		case WM_RBUTTONDOWN:
		{
			self->event.kind = Window_Event::KIND::KIND_MOUSE_BUTTON;
			self->event.mouse_button.b = MOUSE::RIGHT;
			self->event.mouse_button.s = KEY_STATE::DOWN;
			break;
		}

		case WM_RBUTTONUP:
		{
			self->event.kind = Window_Event::KIND::KIND_MOUSE_BUTTON;
			self->event.mouse_button.b = MOUSE::RIGHT;
			self->event.mouse_button.s = KEY_STATE::UP;
			break;
		}

		case WM_MBUTTONDOWN:
		{
			self->event.kind = Window_Event::KIND::KIND_MOUSE_BUTTON;
			self->event.mouse_button.b = MOUSE::MIDDLE;
			self->event.mouse_button.s = KEY_STATE::DOWN;
			break;
		}

		case WM_MBUTTONUP:
		{
			self->event.kind = Window_Event::KIND::KIND_MOUSE_BUTTON;
			self->event.mouse_button.b = MOUSE::MIDDLE;
			self->event.mouse_button.s = KEY_STATE::UP;
			break;
		}

		case WM_MOUSEMOVE:
		{
			self->event.kind = Window_Event::KIND::KIND_MOUSE_MOVE;
			self->event.mouse_move.x = GET_X_LPARAM(lparam);
			self->event.mouse_move.y = GET_Y_LPARAM(lparam);
			break;
		}

		case WM_MOUSEWHEEL:
		{
			self->event.kind = Window_Event::KIND::KIND_MOUSE_WHEEL;
			self->event.mouse_wheel.dir = GET_WHEEL_DELTA_WPARAM(wparam);
			break;
		}

		case WM_SIZE:
		{
			if (self)
			{
				self->event.kind = Window_Event::KIND::KIND_WINDOW_RESIZE;
				self->event.window_resize.width = LOWORD(lparam);
				self->event.window_resize.height = HIWORD(lparam);
				self->width = LOWORD(lparam);
				self->height = HIWORD(lparam);
				break;
			}
		}

		default:
			break;
		}

		return DefWindowProcA(hwnd, msg, wparam, lparam);
	}

	Window
	window_new(unsigned int width, unsigned int height, const char* title)
	{
		IWindow* self = new IWindow;
		self->width = width;
		self->height = height;
		self->title = title;

		WNDCLASSEXA wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEXA));
		wc.cbSize = sizeof(WNDCLASSEXA);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = _window_proc;
		wc.hInstance = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.lpszClassName = "msgpuWindowClass";

		RegisterClassExA(&wc);

		RECT wr = { 0, 0, LONG(self->width), LONG(self->height) };
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

		self->handle = CreateWindowExA(NULL,
			"msgpuWindowClass",
			self->title,
			WS_OVERLAPPEDWINDOW,
			10,
			10,
			wr.right - wr.left,
			wr.bottom - wr.top,
			NULL,
			NULL,
			NULL,
			NULL);

		assert(self->handle != NULL && "ERROR CREATING A WINDOW");

		self->dc = GetDC(self->handle);
		window_pixel_format_set(self);

		ShowWindow(self->handle, SW_SHOW);
		SetForegroundWindow(self->handle);
		SetFocus(self->handle);
		SetWindowLongPtrA(self->handle, GWLP_USERDATA, (LONG_PTR)self);

		return self;
	}

	Window
	window_new_test(void* handle, unsigned int width, unsigned int height, const char* title)
	{
		Window self = new IWindow;
		self->dc = GetDC((HWND)handle);
		self->handle = (HWND)handle;
		self->height = height;
		self->width = width;
		self->title = title;

		return self;
	}

	void
	window_free(Window win)
	{
		bool result = ReleaseDC(win->handle, win->dc);
		assert(result && "ReleaseDC Failed");
		result = DestroyWindow(win->handle);
		assert(result && "DestroyWindow Failed");
		delete win;
	}

	void
	window_swap(Window win)
	{
		bool result = SwapBuffers(win->dc);
		assert(result && "SwapBuffers failed");
	}

	void*
	window_dc(Window win)
	{
		return win->dc;
	}

	void*
	window_handle(Window win)
	{
		return win->handle;
	}

	Window_Event
	window_poll(Window win)
	{
		win->event = Window_Event{};
		MSG msg{};

		//PeekMessageA dispatches incoming msgs, then SendMessage to the win, then this triggers the window procedure
		if (PeekMessageA(&msg, win->handle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		return win->event;
	}

	math::vec2f
	window_size(Window win)
	{
		return math::vec2f{ (float)win->width, (float)win->height };
	}

	void
	window_pixel_format_set(win::Window win)
	{
		//setup the modern pixel format in order to create the modern GL Context
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
			1,								// version number  
			PFD_DRAW_TO_WINDOW |			// support window  
			PFD_SUPPORT_OPENGL |			// support OpenGL  
			PFD_DOUBLEBUFFER |				// double buffered  
			PFD_GENERIC_ACCELERATED,		// accelrated by the driver
			PFD_TYPE_RGBA,					// RGBA type  
			32,								// 32-bit color
			0, 0, 0, 0, 0, 0,				// color bits ignored  
			8,								// 8-bit alpha buffer  
			0,								// shift bit ignored  
			0,								// no accumulation buffer
			0, 0, 0, 0,						// accum bits ignored  
			24,								// 24-bit z-buffer      
			8,								// 8-bit stencil buffer  
			0,								// no auxiliary buffer
			PFD_MAIN_PLANE,					// main layer  
			0,								// reserved  
			0, 0, 0							// layer masks ignored  
		};

		HDC dc = (HDC)win::window_dc(win);
		int pixel_format_id = ChoosePixelFormat(dc, &pfd);
		assert(pixel_format_id && "ChoosePixelFormat failed");

		bool result = SetPixelFormat(dc, pixel_format_id, &pfd);
		assert(result && "pixel format set failed");
	}
};