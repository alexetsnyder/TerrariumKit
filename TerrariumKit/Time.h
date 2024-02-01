#pragma once

#include <chrono>

namespace SysTK
{
	class Time
	{
		public:
			Time();
			~Time();

			double deltaTime() const;

			void update();

		private:
			std::chrono::duration<double> _deltaTime;
			std::chrono::high_resolution_clock::time_point _lastFrame;
	};
}
