#include "actor_client.h"
#include "../ark_actor/ark_common.h"
HANDLE eHandle;
actor_client* actor_client::connect() {
	task = (task_block*)calloc(1, sizeof(task_block));
	
	device->send(IOCTL_IO_ACTOR_CONNECT, (char*)&task, sizeof(task_block*), nullptr, 0);

	return this;
}
actor_client::~actor_client() {
	if (task)
		task->status = REMOVEING;
}

actor_client::actor_client(driver_loader* dHandle): device(dHandle), task(nullptr){
	if (!eHandle) {
		eHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		dHandle->send(IOCTL_IO_ACTOR_NEW, (char*)&eHandle, sizeof(HANDLE), nullptr, 0);
	}
}

actor_client* actor_client::New(driver_loader* dHandle) {
	return new actor_client(dHandle);
}

actor_client* actor_client::push(uintptr_t value, unsigned int size, uintptr_t opearte) {
	task_block_parameter* p = &task->parameter[task->size];
	p->payload = value;
	p->size = size;
	p->opearte = opearte;

	task->size++;
	return this;
}

// 构造任务块并调用
uintptr_t actor_client::run(void* target) {
	task->target = (uintptr_t)target;

	if (task->status == IDLE)
		SetEvent(eHandle);

	while (task->status == RETURNNING);
	task->size = 0;
	return task->receive;
}

actor_client* actor_client::kernel() {
	task->copy = FALSE;
	return this;
}
actor_client* actor_client::process() {
	task->copy = TRUE;
	return this;
}