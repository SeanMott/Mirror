#pragma once

//defines differnt colors

namespace Mirror::Util
{
	struct Color
	{
		float r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;

		//returns white
		static inline Color While() { return { 255, 255, 255, 1.0f }; }
		//returns black
		static inline Color Black() { return { 0.0f, 0.0f, 0.0f, 1.0f }; }

		//returns red
		static inline Color Red() { return { 255.0f, 0.0f, 0.0f, 1.0f }; }
		//returns blue
		static inline Color Blue() { return { 0.0f, 0.0f, 255.0f, 1.0f }; }
		//returns yellow
		static inline Color Yellow() { return { 225.0f, 225.0f, 51.0f, 1.0f }; }

		//returns green
		static inline Color Green() { return { 0.0f, 128.0f, 0.0f, 1.0f }; }
		//returns orange
		static inline Color Orange() { return { 255.0f, 165.0f, 0.0f, 1.0f }; }
		//returns violet
		static inline Color Violet() { return { 238.0f, 130.0f, 238.0f, 1.0f }; }
		//returns purple
		static inline Color Purple() { return { 128.0f, 0.0f, 128.0f, 1.0f }; }

		//returns cyan
		static inline Color Cyan() { return { 0.0f, 255.0f, 255.0f, 1.0f }; }
		//returns pink
		static inline Color Pink() { return { 255.0f, 192.0f, 203.0f }; }
		//returns brown
		static inline Color Brown() { return { 165.0f, 42.0f, 42.0f, 1.0f }; }
		//returns magenta
		static inline Color Magenta() { return { 255.0f, 0.0f, 255.0f, 1.0f }; }

		//returns lawn green
		static inline Color LawnGreen() { return { 124.0f, 252.0f, 0.0f, 1.0f }; }
	};
}