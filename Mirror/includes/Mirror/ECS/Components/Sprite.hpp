//defines a sprite

#ifndef Sprite_h
#define Sprite_h

#include <Mirror\Util\Color.hpp>
#include <stdint.h>

namespace Mirror::ECS::Comp
{
	struct Sprite
	{
		Util::Color color = Util::Color::Black();
		uint32_t textureId = 0;
		bool isActive = true;
	};
}

#endif