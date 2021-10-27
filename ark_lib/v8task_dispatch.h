#pragma once
#include "concurrentqueue/blockingconcurrentqueue.h"
#include "v8cron_job.h"
#include "v8job.h"
#include <unordered_map>
#include "v8vm.h"
#include <thread>
#include <atomic>

class v8TaskDispatch {
public:
	v8TaskDispatch();
	~v8TaskDispatch();
	v8TaskDispatch& pushJob(const v8job& job);
	const std::string&& pushCronJob(const v8CronJob& cron_job);
	void removeCronJob(const std::string& cron_job_id);
	void start();
	void stop();
	v8job&& waitFor();

	static std::shared_ptr<v8TaskDispatch> Instance();

	// ����״̬
	std::atomic_bool running;
	// worker
	std::unordered_map<std::string, std::unique_ptr<std::thread >> worker_list;

private:
	// ����������
	unsigned int worker_number;

	// �ȴ�����
	void waitFor(v8vm& vm);

	// ����ִ�ж���
	moodycamel::BlockingConcurrentQueue<v8job> task_queue;
	// �����������
	moodycamel::BlockingConcurrentQueue<v8job> result_queue;
	// ��ʱ�����¼
	std::unordered_map<std::string, v8CronJob> cron_job_list;
	
};