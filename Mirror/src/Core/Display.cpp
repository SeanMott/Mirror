#include <Mirror\Core\Display.hpp>

#include <stdio.h>

static bool GLFW_IS_INIT = false;

//-----------------GLFW METHOD CALL BACKS-------------//
//logs errors
void GLFWErrorCallBack(int error, const char* description);
//allows the window to resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//allows the exit window button to be pressed || closes the window
void window_close_callback(GLFWwindow* window);
//allows mouse move events to be triggered
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
//allows mouse scroll events
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//allows keyboard pressed events to be triggered
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//allows mouse pressed events to be triggered
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
//char callback allows typing
void character_callback(GLFWwindow* window, unsigned int keycode);

Mirror::Core::Display::Display(const char* name, int32_t _width, int32_t _height)
	:width(_width), height(_height)
{
	//init GLFW
	if (!GLFW_IS_INIT)
	{
		if (!glfwInit())
		{
			return;
			//LogFatalError("GLFW Init", "Failed to init GLFW!\n");
		}

		GLFW_IS_INIT = true;
	}

	//window settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, true);

	//makes window
	window = glfwCreateWindow(_width, _height, name, NULL, NULL);
	if (!window)
	{
		return;
		//LogFatalError("NULL GLFW Window", "Failed to make a GLFW window!");
	}
	glfwMakeContextCurrent(window);

	//set method call backs
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 	//resize
	glfwSetWindowCloseCallback(window, window_close_callback); //closes the window when the exit button is pressed
	glfwSetKeyCallback(window, key_callback);//key input
	glfwSetMouseButtonCallback(window, mouse_button_callback); //mouse input
	glfwSetCursorPosCallback(window, cursor_position_callback);//mouse move
	glfwSetScrollCallback(window, scroll_callback);//mouse scroll
	glfwSetCharCallback(window, character_callback); //type input

	//make context
	Glfix_Context_CreateInfo glfixInfo;
	Glfix_Context_InitCreateInfo(&glfixInfo);
	glfixInfo.GLFWIsInit = true;
	glfixInfo.viewPortHeight = height;
	glfixInfo.viewPortWidth = width;
	context = Glfix_Context_Create(&glfixInfo);
	if (!context)
	{
		return;
		//LogFatalError("Failed Context", "Failed to make a Context");
	}

	//if all works
	isRunning = true;

	//link to window events
	//Event::WindowResizeEvent::AddMethod(this, &Display::Resize);
	//Event::WindowCloseEvent::AddMethod(this, &Display::Destroy);
}

void Mirror::Core::Display::Shutdown()
{
	glfwSetWindowShouldClose(window, true);
	isRunning = false;
}

void Mirror::Core::Display::Destroy()
{
	if (isRunning)
		Shutdown();

	Glfix_Context_Destroy(context);
	glfwDestroyWindow(window);
	glfwTerminate();
}

/*float Mirror::Core::Display::CalDeltaTime()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	return deltaTime;
}*/

//------GLFW CALL BACK METHODS-----------//
//logs errors
void GLFWErrorCallBack(int error, const char* description)
{
	//LogError(((std::string)"GLFW (" + std::to_string(error) + ")").c_str(), description);
	printf("GLFW Error (%i): %s\n", error, description);
}

//allows the window to resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//Smok::Event::WindowResizeEvent::Call(width, height);
}

//allows the exit window button to be pressed || closes the window
void window_close_callback(GLFWwindow* window)
{
	//Smok::Event::WindowCloseEvent::Call();
}

//allows mouse move events to be triggered
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Smok::Event::MouseMoveEvent::Call(xpos, ypos);
}

//allows mouse scroll events
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//Smok::Event::MouseScrollEvent::Call(xoffset, yoffset);
}

//allows keyboard pressed events to be triggered
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//send to input
	/*switch (action)
	{
	case GLFW_PRESS:
		Smok::Event::KeyPressEvent::Call(key, false);
		break;

	case GLFW_REPEAT:
		Smok::Event::KeyPressEvent::Call(key, true);
		break;

	case GLFW_RELEASE:
		Smok::Event::KeyReleasedEvent::Call(key);
		break;
	}*/
}

//char callback allows typing
void character_callback(GLFWwindow* window, unsigned int keycode)
{
	//Smok::Event::KeyTypedEvent::Call(keycode);
}

//allows mouse pressed events to be triggered
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//send to input
	/*switch (action)
	{
	case GLFW_PRESS:
		Smok::Event::MousePressEvent::Call(button);
		break;

	case GLFW_RELEASE:
		Smok::Event::MouseReleasedEvent::Call(button);
		break;
	}*/
}