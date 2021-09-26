#pragma once
#ifdef WIN_KERNEL
#include <ntddk.h>
#else
#include <Windows.h>
#endif

#include "stdlib.h"

enum TASK_RUN_STATUS {
	IDLE,				// 空闲中
	RUNNING,			// 运行中
	RETURNNING,			// 取值中
	REMOVEING = 404		// 删除中（移除任务块时，需要判断 self->task_queue 是否为 remove 对象）
}TASK_RUN_STATUS;

typedef struct task_block_parameter{
	uintptr_t	payload;
	uintptr_t	size;
	uintptr_t	opearte;	// 0 in, 1 out
}task_block_parameter;

typedef struct task_block {
	uintptr_t				status;					// 任务状态 0，空闲 1,（等待）执行 2,（等待）取值 404,删除任务块
	
	uintptr_t				target;					// 调用目标
	task_block_parameter	parameter[20];			// 参数列
	uintptr_t				size;				// 参数数量

	uintptr_t				receive;				// 返回列
	uintptr_t				copy;					// 是否需要拷贝参数
}task_block;

typedef struct actor_server actor_server;

typedef actor_server* (*pfn_serve)(actor_server*);
typedef actor_server* (*pfn_accpet)(actor_server*, task_block* block);

typedef struct actor_server {
	array_list*		task_queue;		// 任务块链表指针
	HANDLE			event_handle;	// 事件句柄
	pfn_serve		serve;			// 开始服务
	pfn_accpet		accpet;			// 同意服务（注册服务）
	uintptr_t		status;			// 0 空闲， 1 运行
#ifdef WIN_KERNEL
	PKEVENT			event_object;
#else
	void* event_object;
#endif
}actor_server;