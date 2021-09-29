#include "actor_client.h"
#include "../ark_actor/ark_common.h"
HANDLE eHandle;
actor_client* actor_client::connect() {
	task = (task_block*)calloc(1, sizeof(task_block));
	if (task) {

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
task(nullptr),
copy(FALSE){
	if (!eHandle) {
		eHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
#ifdef _DEBUG
		printf("Events:%p\n", eHandle);
#endif
		dHandle->send(IOCTL_IO_ACTOR_NEW, (char*)&eHandle, sizeof(HANDLE), nullptr, 0);
	}
	load_module = new driver();
}

actor_client* actor_client::New(driver_loader* dHandle) {
	return new actor_client(dHandle);
}

actor_client* actor_client::NewProcess(driver_loader* dHandle) {
	actor_client* self = new actor_client(dHandle);

	self->copy = true;

	return self;
}

InternalServices* actor_client::services() {
	return task->internal_services;
}

actor_client* actor_client::push(void* value, unsigned int size, uintptr_t opearte) {
	task_block_parameter* p = &task->parameter[task->size];
	p->payload = (uintptr_t)value;
	p->size = size;
	p->opearte = opearte;

	task->size++;
	return this;
}

// 构造任务块并调用
uintptr_t actor_client::run(void* target, unsigned char interrupt_level) {
	task->target = (uintptr_t)target;
	task->interrupt_level = interrupt_level;
#ifdef _DEBUG
	printf("Task Status ： %llu\n", task->status);
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