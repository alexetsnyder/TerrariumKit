#pragma once

#include <chrono>

namespace SysTK
{
	class Time
	{
		public:
			Time() = delete;
			Time(const Time&) = delete;

			static void start();

			static double deltaTime();
			static double fixedDeltaTime();

			static void update();

		private:
			static double framesPerSecond_;
			static double secPerFrame_;
			static std::chrono::duration<double> deltaTime_;
			static std::chrono::steady_clock::time_point lastFrame_;
	};
}
