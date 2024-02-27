#include "JobManager.h"

#include "Time.h"

namespace SysTK
{
	void Invoke(std::function<void()>& jFunction, double jTimer)
	{
		Job job;
		job.jTimer = jTimer;
		job.jFunction = jFunction;
		JobManager::addJob(job);
	}

	std::list<Job> JobManager::jobs_{};

	void JobManager::addJob(Job job)
	{
		jobs_.push_back(job);
	}

	void JobManager::update()
	{
		auto it = jobs_.begin();
		while (it != jobs_.end())
		{
			it->jTimer -= Time::fixedDeltaTime();
			if (it->jTimer <= 0.0)
			{
				it->jFunction();
				it = jobs_.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}