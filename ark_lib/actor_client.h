#pragma once
#include "../ark_actor/actor_common.h"
#include "../ark_lib/driver_loader.h"

class actor_client {
public:
	actor_client(driver_loader*);
	~actor_client();

	actor_client* push(uintptr_t value, unsigned int size = sizeof(uintptr_t), uintptr_t opearte = 0);
	uintptr_t run(void* target);

	actor_client* kernel();
	actor_client* process();

	actor_client* connect();	// 连接服务器
	static actor_client* New(driver_loader*);
private:
	task_block* task;			// 所属任务块
	driver_loader* device;		// 设备对象
};