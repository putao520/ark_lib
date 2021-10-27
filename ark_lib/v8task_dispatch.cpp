#include "v8task_dispatch.h"
#include "guid_until.h"
#include "info.h"

v8TaskDispatch::v8TaskDispatch() :
	worker_number(ark::system::GetCpuCoreTotalCount() / 2),
	task_queue(300),
	result_queue(100),
	running(false) {
}

v8TaskDispatch::~v8TaskDispatch() {
	this->stop();
}

v8TaskDispatch& v8TaskDispatch::pushJob(const v8job& job) {
	// 任务允许丢失
	task_queue.try_enqueue(job);
	return *this;
}

const std::string&& v8TaskDispatch::pushCronJob(const v8CronJob& cron_job) {
	std::string ctid;
	do {
		ctid = guid_until::getRandomUUID();
	} while (cron_job_list.count(ctid));
	cron_job_list[ctid] = cron_job;
	return std::move(ctid);
}

void  v8TaskDispatch::removeCronJob(const std::string& cron_job_id) {
	if (cron_job_list.count(cron_job_id))
		cron_job_list.erase(cron_job_id);
}

void v8TaskDispatch::waitFor(v8vm& vm) {
	v8job job;
	task_queue.wait_dequeue(job);
	job.run(vm);
	// 回传允许失败
	result_queue.try_enqueue(job);
}

v8job&& v8TaskDispatch::waitFor() {
	v8job job;
	result_queue.wait_dequeue(job);
	return std::move(job);
}

void v8TaskDispatch::start() {
	if (!running) {
		running = true;
		for (UINT32 i = 0; i < worker_number; i++) {
			const UINT32 task_id = i;
			std::unique_ptr<std::thread> _thread = std::make_unique<std::thread>([&, task_id] {
				v8vm vm;
				while(this->running)
					this->waitFor(vm);
				this->worker_list.erase(std::to_string(task_id));
				});
			worker_list[std::to_string(i)] = std::move(_thread);
			_thread->detach();
		}
	}
}

void v8TaskDispatch::stop() {
	if (running) {
		this->running = false;
		while (this->worker_list.size())
			std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

std::shared_ptr<v8TaskDispatch> v8TaskDispatch::Instance() {
	return std::make_shared<v8TaskDispatch>();
}