#pragma once
#ifdef WIN_KERNEL
#include "../ark_sys/rtl.h"
#else
#include <Windows.h>
#endif

#include "internal.h"
#include "stdlib.h"
#include "../ark_sys/gs_array.h"

EXTERN_C_START

typedef enum TASK_RUN_STATUS {
	TASK_IDLE,				// 空闲中、等待任务
	TASK_RUNNING,			// 运行中
	TASK_RETURNNING,		// 取值中
	TASK_REMOVEING = 404,	// 删除中（移除任务块时，需要判断 self->task_queue 是否为 remove 对象）
	TASK_WAITTINGFREE,		// 等待卸载
}TASK_RUN_STATUS;

typedef enum TASK_RUN_ERROR {
	TASK_ERROR_SUCCESS,		// 成功
	TASK_ERROR_TARGET		// 目标地址无效
}TASK_RUN_ERROR;

typedef struct task_block_parameter{
	uintptr_t	payload;
	uintptr_t	size;
	uintptr_t	opearte;	// 0 in, 1 out
}task_block_parameter;

typedef struct task_block {
	uintptr_t				status;					// 任务状态 0，空闲 1,（等待）执行 2,（等待）取值 404,删除任务块
	uintptr_t				error;					// 执行错误
	
	uintptr_t				target;					// 调用目标
	task_block_parameter	parameter[20];			// 参数列
	uintptr_t				size;					// 参数数量

	uintptr_t				receive;				// 返回列
	uintptr_t				copy;					// 是否需要拷贝参数
	uintptr_t				interrupt_level;		// 执行中断等级
	InternalServices* internal_services;			// 内置服务表
}task_block;

EXTERN_C_END