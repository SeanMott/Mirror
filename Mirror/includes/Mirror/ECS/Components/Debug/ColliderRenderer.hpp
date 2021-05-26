#pragma once

//defines data for rendering a collider

#include <Mirror\Util\Color.hpp>

namespace Mirror::ECS::Comp
{
	//the type
	enum class ColliderType
	{
		Box = 0
	};

	//the comp
	struct ColliderRenderer
	{
		ColliderType type = ColliderType::Box;
		Util::Color renderColor = Util::Color::LawnGreen(); //stores the color for rendering
		bool isActive = true;
	};
}