#pragma once

//defines the input manager

#include <Mirror\Platforms\Keycodes.hpp>
#include <GLFW\glfw3.h>

namespace Mirror::Core
{
	class Input
	{
		//vars
	private:

		GLFWwindow* window = nullptr;

		//add map keys

		//methods
	public:

		//Constructor
		Input(GLFWwindow* win)
			:window(win) {}

		//checks if a key is down
		inline bool GetKey(Platform::Keycode key) { return glfwGetKey(window, Platform::ConvertToNativeKeycode(key)); }
	};
}