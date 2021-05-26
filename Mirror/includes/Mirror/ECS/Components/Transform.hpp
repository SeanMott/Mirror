//defines a transform component

#ifndef Transform_h
#define Transform_h

#include <Mirror\Util\Math.hpp>

namespace Mirror::ECS::Comp
{
	struct Transform
	{
		Util::Vector3<float> position = { 0.0f, 0.0f, 0.0f };
		Util::Vector3<float> rotation = { 0.0f, 0.0f, 0.0f };
		Util::Vector3<float> scale = { 1.0f, 1.0f, 1.0f };
	};
}

#endif