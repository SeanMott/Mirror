//defines a sprite

#ifndef Sprite_h
#define Sprite_h

#include <SmokCore\Util\Color.hpp>
#include <stdint.h>

namespace Mirror::ECS::Comp
{
	struct Sprite
	{
		SmokCore::Util::Color color = SmokCore::Util::Color::Black();
		uint32_t textureId = 0;
		bool isActive = true;
	};
}

#endif