
#include <SmokCore\Core\Engine.hpp>

int main(void)
{
	//init Engine
	SmokCore::Core::EngineCreateInfo engineInfo;
	engineInfo.displayInfo.enableDepthBuffer = false;
	engineInfo.displayInfo.lockMouse = false;
	engineInfo.displayInfo.name = "Land Unknown";
	SmokCore::Core::Engine* engine = SmokCore::Core::Engine::Init(engineInfo);

	//load assets

	//systems

	//Game load

	//game loop
	while (engine->Display()->IsRunning())
	{
		//input
		engine->Display()->PollEvents();

		//exits application
		if (engine->Input()->GetKey(SmokCore::Input::Keycode::ESCAPE))
			break;

		//update

		//render
		engine->Display()->Clear();

		//render stuff

		engine->Display()->SwapRenderBuffer();
	}

	//clean up

	//game data

	//systems

	//engine
	engine = nullptr;
	SmokCore::Core::Engine::Shutdown();

	return 0;
}