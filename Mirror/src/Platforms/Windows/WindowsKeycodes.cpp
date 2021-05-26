#include <Mirror\Platforms\Keycodes.hpp>

#include <GLFW\glfw3.h>
#include <stdio.h>

int32_t Mirror::Platform::ConvertToNativeKeycode(Mirror::Platform::Keycode key)
{
	switch (key)
	{
	case Mirror::Platform::Keycode::Escape:
		return GLFW_KEY_ESCAPE;
	case Mirror::Platform::Keycode::Space:
		return GLFW_KEY_SPACE;
	case Mirror::Platform::Keycode::J:
		return GLFW_KEY_J;
	default:
		printf("Mirror: Error: Invalid Keycode || Keycode could not be converted to native platform!\n");
		return -1;
	}
}