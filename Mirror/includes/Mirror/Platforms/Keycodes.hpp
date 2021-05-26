#pragma once

//defines platform agnostic key codes

#include <stdint.h>

namespace Mirror::Platform
{
	enum class Keycode
	{
		Escape = 0,
		Space,

		J,
	};

	//converts keycodes to the native ones
	int32_t ConvertToNativeKeycode(Keycode key);
}