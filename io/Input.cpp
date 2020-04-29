#include "io/Input.h"
#include "io/Event.h"

namespace io
{
	void
	input_process_event(Input& input, Event event)
	{
		switch (event.kind)
		{
			case Event::KIND::KIND_MOUSE_MOVE:
			{
				input.mouse_x = event.mouse_move.x;
				input.mouse_y = event.mouse_move.y;
				break;
			}

			case Event::KIND::KIND_KEYBOARD_KEY:
			{
				if(event.keyboard_key.s == KEY_STATE::DOWN)
					input.keyboard[(int)event.keyboard_key.k] = true;
				else
					input.keyboard[(int)event.keyboard_key.k] = false;
				break;
			}

			case Event::KIND::KIND_MOUSE_BUTTON:
			{
				if (event.mouse_button.s == KEY_STATE::DOWN)
					input.mouse[(int)event.mouse_button.b] = true;
				else
					input.mouse[(int)event.mouse_button.b] = false;
				break;
			}
		

			case Event::KIND::KIND_MOUSE_WHEEL:
			{
				input.wheel_dir = event.mouse_wheel.dir;
				break;
			}
			default:
				break;
		}
	}

	math::vec2f
	input_mouse_delta(const Input & i)
	{
		return math::vec2f{ (float)i.mouse_x - i.pmouse_x, (float)i.pmouse_y - i.mouse_y };
	}

	void
	input_update(Input & i)
	{
		i.pmouse_x = i.mouse_x;
		i.pmouse_y = i.mouse_y;
		i.wheel_dir = 0;
	}
};