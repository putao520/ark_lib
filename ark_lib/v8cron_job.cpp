#include "v8cron_job.h"

v8CronJob::v8CronJob(const v8job& job): _job(job) {
	_delay = std::chrono::milliseconds(0);
	_last = std::chrono::system_clock::now();

	_job.setOnReturn([this](std::string r)->void {
		this->_last = std::chrono::system_clock::now();
		});
}

v8CronJob::v8CronJob(v8CronJob&& job): _job(job._job), _delay(job._delay), _last(job._last){
}

v8CronJob::v8CronJob(const v8CronJob& job): _job(job._job), _delay(job._delay), _last(job._last) {
}

v8CronJob& v8CronJob::operator=(const v8CronJob& job) {
	this->_job = job._job;
	this->_delay = job._delay;
	this->_last = job._last;
	return *this;
}

v8CronJob& v8CronJob::duration(const std::chrono::milliseconds& delay) {
	_delay = delay;
	return *this;
}

const v8job& v8CronJob::getJob() {
	return _job;
}