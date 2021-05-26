//defines a 2D camera for Mirror

#ifndef Camera_h
#define Camera_h

namespace Mirror::ECS::Comp
{
	struct Camera
	{
		float viewWidthMax = 800.0f, viewHeightMax = 600.0f;
		float viewWidthMin = 0.0f, viewHeightMin = 0.0f;
		float nearFieldClipping = -1.0f, farFieldClipping = 1.0f;
		bool isActive = true;
	};
}

#endif