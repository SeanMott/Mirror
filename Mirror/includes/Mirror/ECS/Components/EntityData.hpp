#pragma once

//defines a component for storing a copy of the entity data

#include <string>

namespace Mirror::ECS::Comp
{
	struct EntityData
	{
		std::string name = "Entity", tag = "Default", layer = "Default";
		bool isActive = true;
	};
}