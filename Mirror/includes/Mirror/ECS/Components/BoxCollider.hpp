#pragma once

//defines a box collider

#include <Mirror\Util\Math.hpp>
#include <Mirror\ECS\Entity.hpp>

namespace Mirror::ECS::Comp
{
	struct BoxCollider
	{
		Util::Vector2<float> size = { 1.0f, 1.0f };
		Util::Vector2<float> posOffset = { 0.0f, 0.0f };

		void(*funcCallback)(Entity* self, Entity* other) = nullptr; //the function called when it makes contact
		
		bool isTrigger = false;
		bool isActive = true;
	};
}