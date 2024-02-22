#pragma once

#include <functional>
#include <list>

namespace SysTK
{
	void Invoke(std::function<void()>& jFunction, double jTimer);

	struct Job
	{
		double jTimer;
		std::function<void()> jFunction;
	};

	class JobManager
	{
		public:
			JobManager() = delete;
			JobManager(const JobManager&) = delete;

			static void addJob(Job job);
			static void update();

		private:
			static std::list<Job> jobs_;
	};
}
