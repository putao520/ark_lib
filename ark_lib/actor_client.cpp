#include "actor_client.h"
#include "../ark_actor/ark_common.h"
#include <assert.h>

#ifdef _DEBUG
#include "debug_until.h"
#endif

HANDLE eHandle;
actor_client* actor_client::connect() {
	task = (task_block*)calloc(1, sizeof(task_block));
	if (task) {

#ifdef _DEBUG
		__pauseDebug("actor_client::connect =>%p\n", task);
#endif

		task->internal_services = (InternalServices*)calloc(1, sizeof(InternalServices));

		device->send(IOCTL_IO_ACTOR_CONNECT, (char*)&task, sizeof(task_block*), nullptr, 0);
	}
	return this;
}
actor_client::~actor_client() {
	if (task)
		task->status = TASK_REMOVEING;
	if (load_module)
		delete load_module;
}

actor_client::actor_client(driver_loader* dHandle): device(dHandle), 
task(nullptr){
	if (!eHandle) {
		eHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
#ifdef _DEBUG
		printf("Events:%p\n", eHandle);
		__pauseDebug("Events:%p\n", eHandle);
#endif

		dHandle->send(IOCTL_IO_ACTOR_NEW, (char*)&eHandle, sizeof(HANDLE), nullptr, 0);
	}
	load_module = new driver();
}

actor_client* actor_client::New(driver_loader* dHandle) {
	return new actor_client(dHandle);
}

InternalServices* actor_client::services() {
	return task->internal_services;
}

actor_client* actor_client::push(void* value) {
	
	assert(task != nullptr);

	/*
	task_block_parameter* p = &task->parameter[task->size];
	p->payload = (uintptr_t)value;
	p->size = size;
	*/

	task->parameter[task->size] = (uintptr_t)value;

	task->size++;
	return this;
}

// 构造任务块并调用
uintptr_t actor_client::run(void* target, unsigned char interrupt_level) {
	task->target = (uintptr_t)target;
	task->interrupt_level = interrupt_level;
#ifdef _DEBUG
	printf("Task Status ： %llu\n", task->status);
	__pauseDebug("Task Status ： %llu\n", task->status);
#endif
	uintptr_t _status = task->status;
	task->status = TASK_RUNNING;
	// if (_status == TASK_IDLE)
	SetEvent(eHandle);
	// ---------r3->r0----------
	while (task->status != TASK_RETURNNING);

	auto r = task->receive;
	task->size = 0;
	task->status = TASK_IDLE;
	return r;
}

uintptr_t actor_client::runAsRva(const char* name, void* rva, unsigned char irql /* = 1 */) {
	char* target = (char *)load_module->module(name) + (uintptr_t)rva;
#ifdef _DEBUG
	printf("call target:%p\n", target);
	__pauseDebug("call target:%p\n", target);
#endif
	if (!target)
		return 0;
	auto r =  run(target, irql);
	// 执行时发生错误
	if (task->error != TASK_ERROR_SUCCESS)
		dispatch_error(name);

	return r;
}

void actor_client::dispatch_error(const char* name) {
	switch (task->error){
	case TASK_ERROR_TARGET:
		if (name)
			load_module->remove(name);
		break;
	default:
		break;
	}
	task->error = TASK_ERROR_SUCCESS;
}