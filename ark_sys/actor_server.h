#pragma once
#include "../ark_actor/actor_common.h"

EXTERN_C_START


typedef struct actor_server actor_server;

typedef actor_server* (*pfn_serve)(actor_server*);
typedef actor_server* (*pfn_accpet)(actor_server*, task_block* block);

typedef struct actor_server {
	array_list* task_queue;		// 任务块链表指针
	HANDLE			event_handle;	// 事件句柄
	pfn_serve		serve;			// 开始服务
	pfn_accpet		accpet;			// 同意服务（注册服务）
	uintptr_t		status;			// 0 空闲， 1 运行
#ifdef WIN_KERNEL
	PKEVENT			event_object;
#else
	void* event_object;
#endif
	InternalServices* internal_services;	// 内置服务表
}actor_server;

void* CallActorServiceGen(task_block_parameter* parameter, uintptr_t size, void* target);
actor_server* NewActorServer(HANDLE eHandle);
void DeleteActorServer(actor_server* self);

EXTERN_C_END