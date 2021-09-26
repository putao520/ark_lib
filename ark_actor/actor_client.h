#pragma once
#include "actor_common.h"
#include "../ark_lib/driver_loader.h"

class actor_client {
public:
	actor_client(driver_loader*);
	~actor_client();

	template<typename... Targs>
	uintptr_t run(void* target, Targs... args);

	actor_client* connect();	// ���ӷ�����
	static actor_client* New(driver_loader*);
private:
	task_block* task;			// ���������
	driver_loader* device;		// �豸����
};