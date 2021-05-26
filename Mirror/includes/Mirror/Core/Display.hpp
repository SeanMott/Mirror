//defines a display

#ifndef Display_h
#define Display_h

#include <Mirror\Util\Math.hpp>

#include <Glfix\Context.h>
#include <GLFW\glfw3.h>

namespace Mirror::Core
{
	class Display
	{
		//vars
	private:

		int32_t width = 0, height = 0;
		bool isRunning = false;

		GLFWwindow* window = nullptr;
		Glfix_Context* context = nullptr;

		//methods
	public:

		//Constructor
		Display(const char* name = "Game", const int32_t _width = 800, const int32_t _height = 600);
		//Deconstructor
		~Display() { Destroy(); }

		//destroys the display
		void Destroy();
		//shutsdown the display
		void Shutdown();

		
		//polls input events
		inline void PollEvents() const { glfwPollEvents(); }

		//clears the screen
		inline void Clear() const { if (!context) return; Glfix_Context_Clear(context); }
		//swaps the render buffer
		inline void SwapRenderBuffer() const { if (!window) return;  glfwSwapBuffers(window); }
		//sets the clear color
		inline void SetClearColor(float red, float green, float blue, float alpha = 1.0f) const { Glfix_Context_SetClearColor(red, green, blue, alpha); }
		//sets the clear color
		inline void SetClearColor(Mirror::Util::Vector4<float> color) const { Glfix_Context_SetClearColorVec4(Mirror_Math_TypePunP(color, float)); }

		//sets the window height
		inline void SetHeight(int32_t h) { height = h; }
		//sets the window width
		inline void SetWidth(int32_t w) { width = w; }
		//resizes the display
		inline void Resize(int32_t w, int32_t h)
		{
			width = w; height = h;
			Glfix_Context_SetViewPort(0, 0, w, h);
		}

		//gets the window
		inline GLFWwindow* GetWindow() const { return window; }
		//gets the context
		inline Glfix_Context* GetContext() const { return context; }
		//gets the window height
		inline int32_t GetHeight() const { return height; }
		//gets the window width
		inline int32_t GetWidth() const { return width; }
		//gets if the window is running
		inline bool IsRunning() const { return isRunning; }
	};
}

#endif