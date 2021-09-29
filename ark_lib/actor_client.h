#pragma once
#include "../ark_actor/actor_common.h"
#include "../ark_lib/driver_loader.h"
#include "../ark_actor/internal.h"
#include "driver.h"

class actor_client {
public:
	bool copy;

	actor_client(driver_loader*);
	~actor_client();

	uintptr_t run(void* target, unsigned char irql = 1);
	uintptr_t runAsRva(const char* name, void* rva, unsigned char irql = 1);

	InternalServices* services();

	actor_client* connect();	// ���ӷ�����
	static actor_client* New(driver_loader*);
	static actor_client* NewProcess(driver_loader*);

	actor_client* push(void* value, unsigned int size = sizeof(uintptr_t), uintptr_t opearte = 0);

	template<typename T>
	actor_client* push(T value, uintptr_t opearte = 0){
		unsigned int size = sizeof(T);
		return push((void*)value, size, opearte);
	}


private:
	task_block* task;			// ���������
	driver_loader* device;		// �豸����
	driver* load_module;		// �����������

	void dispatch_error(const char*);
};