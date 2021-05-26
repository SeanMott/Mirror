#include <Mirror\Util\Time.hpp>

#include <GLFW\glfw3.h>

float Mirror::Util::Time::lastFrame = 0.0f;
float Mirror::Util::Time::deltaTime = 0.0f;
float Mirror::Util::Time::fixedDeltaTime = 0.0f;

float Mirror::Util::Time::CalDeltaTime()
{
	float currentTime = glfwGetTime();
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
	return deltaTime;
}