#pragma once

//defines funtions for time and randomization

#include <stdint.h>
#include <stdlib.h>
#include <ctime>

namespace Mirror::Util
{
	class Time
	{
		//vars
	private:

		static float lastFrame, deltaTime, fixedDeltaTime;

		//methods
	public:

		//cals delta time
		static float CalDeltaTime();
		//cals fixed delta time

		//gets the time on the system
		//gets frame time

		//seeds a random number genarater
		static inline void SeedRNG(const int32_t seed) { srand(seed); }
		//seeds a random number genarater using time
		static inline void SeedRNG() { srand(time(NULL)); }
		//gets a random number || mix (in scope) max (outside the scope)
		static inline int32_t RNG(const int32_t min, const int32_t max) { return rand() % max + min; }

		//gets the delta time
		static inline float DeltaTime() { return deltaTime; }
		//gets the fixed time delta
	};
}