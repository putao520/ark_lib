#pragma once
#include "v8job.h"
#include <chrono>

class v8CronJob{
public:
	v8CronJob() {}
	v8CronJob(const v8job& job);

	v8CronJob(v8CronJob&& job);
	v8CronJob(const v8CronJob& job);

	v8CronJob& operator=(const v8CronJob& job);

	v8CronJob& duration(const std::chrono::milliseconds&);
	const v8job& getJob();
private:
	std::chrono::milliseconds _delay;
	std::chrono::system_clock::time_point _last;
	v8job _job;
};