#pragma once

//defines a box collider

#include <SmokCore\Util\Math.hpp>
#include <SmokCore\ECS\Entity.hpp>

namespace Mirror::ECS::Comp
{
	struct BoxCollider
	{
		SmokCore::Util::Vector2<float> size = { 1.0f, 1.0f };
		SmokCore::Util::Vector2<float> posOffset = { 0.0f, 0.0f };

		void(*funcCallback)(SmokCore::ECS::Entity* self, SmokCore::ECS::Entity* other) = nullptr; //the function called when it makes contact
		
		bool isTrigger = false;
		bool isActive = true;
	};
}