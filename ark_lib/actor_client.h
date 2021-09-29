#pragma once
#include "../ark_actor/actor_common.h"
#include "../ark_lib/driver_loader.h"
#include "../ark_actor/internal.h"
#include "driver.h"

using namespace std;
class actor_client {
public:
	actor_client(driver_loader*);
	~actor_client();

	uintptr_t run(void* target, unsigned char irql = 1);
	uintptr_t runAsRva(const char* name, void* rva, unsigned char irql = 1);

	InternalServices* services();

	actor_client* connect();	// 连接服务器
	static actor_client* New(driver_loader*);
	
	actor_client* push(void* value);
	
	template<typename T>
	actor_client* push(T value){
		if constexpr (is_integral<T>::value && sizeof(T) < sizeof(uintptr_t)) {
			char* p = nullptr;
			p += value;
			return push((void*)p);
		}
		else {
			return push((void*)value);
		}
	}
	

private:
	task_block* task;			// 所属任务块
	driver_loader* device;		// 设备对象
	driver* load_module;		// 驱动载入对象

	void dispatch_error(const char*);
};