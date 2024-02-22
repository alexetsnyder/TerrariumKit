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

			static void update();

		private:
			static std::chrono::duration<double> deltaTime_;
			static std::chrono::high_resolution_clock::time_point lastFrame_;
	};
}
