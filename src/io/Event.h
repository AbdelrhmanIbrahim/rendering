#pragma once

//keyboard enums and their mapping taken from Break engine, an incomplete game engine I worked on with a friend years ago 
//(thanks mostafa)
namespace io
{
	enum struct KEYBOARD
	{
		BACKSPACE,
		TAB,
		ENTER,
		ESC,
		SPACE,
		MINUS,
		PERIOD,
		COMMA,
		NUM_0,
		NUM_1,
		NUM_2,
		NUM_3,
		NUM_4,
		NUM_5,
		NUM_6,
		NUM_7,
		NUM_8,
		NUM_9,
		COLON,
		SEMICOLON,
		EQUAL,
		FORWARD_SLASH,
		LEFT_BRACKET,
		RIGHT_BRACKET,
		BACKSLASH,
		BACKQUOTE,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		DLT,
		NUMPAD_0,
		NUMPAD_1,
		NUMPAD_2,
		NUMPAD_3,
		NUMPAD_4,
		NUMPAD_5,
		NUMPAD_6,
		NUMPAD_7,
		NUMPAD_8,
		NUMPAD_9,
		NUMPAD_PERIOD,
		NUMPAD_DIVIDE,
		NUMPAD_PLUS,
		NUMPAD_MINUS,
		NUMPAD_ENTER,
		NUMPAD_EQUALS,
		NUMPAD_MULTIPLY,
		UP,
		DOWN,
		RIGHT,
		LEFT,
		INSERT,
		HOME,
		END,
		PAGEUP,
		PAGEDOWN,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		NUM_LOCK,
		CAPS_LOCK,
		SCROLL_LOCK,
		RIGHT_SHIFT,
		LEFT_SHIFT,
		RIGHT_CTRL,
		LEFT_CTRL,
		LEFT_ALT,
		RIGHT_ALT,
		LEFT_META,
		RIGHT_META,
		COUNT
	};

	enum struct MOUSE
	{
		LEFT,
		MIDDLE,
		RIGHT,
		COUNT
	};

	enum struct KEY_STATE
	{
		UP,
		DOWN,
		COUNT
	};

	struct Event
	{
		enum KIND
		{
			KIND_NONE,
			KIND_KEYBOARD_KEY,
			KIND_MOUSE_BUTTON,
			KIND_MOUSE_MOVE,
			KIND_MOUSE_WHEEL,
			KIND_WINDOW_RESIZE,
			KIND_WINDOW_CLOSE,
			KIND_WINDOW_UPDATE,
		};

		KIND kind;

		union
		{
			struct
			{
				KEYBOARD k;
				KEY_STATE s;
			} keyboard_key;

			struct
			{
				MOUSE b;
				KEY_STATE s;
			} mouse_button;

			struct
			{
				int x, y;
			} mouse_move;

			struct
			{
				int dir;
			} mouse_wheel;

			struct
			{
				int width, height;
			} window_resize;

			struct
			{
				bool close;
			} window_close;
		};
	};
};