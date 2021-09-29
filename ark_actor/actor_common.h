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
	TASK_IDLE,				// �����С��ȴ�����
	TASK_RUNNING,			// ������
	TASK_RETURNNING,		// ȡֵ��
	TASK_REMOVEING = 404,	// ɾ���У��Ƴ������ʱ����Ҫ�ж� self->task_queue �Ƿ�Ϊ remove ����
	TASK_WAITTINGFREE,		// �ȴ�ж��
}TASK_RUN_STATUS;

typedef enum TASK_RUN_ERROR {
	TASK_ERROR_SUCCESS,		// �ɹ�
	TASK_ERROR_TARGET		// Ŀ���ַ��Ч
}TASK_RUN_ERROR;

typedef struct task_block_parameter{
	uintptr_t	payload;
	uintptr_t	size;
	uintptr_t	opearte;	// 0 in, 1 out
}task_block_parameter;

typedef struct task_block {
	uintptr_t				status;					// ����״̬ 0������ 1,���ȴ���ִ�� 2,���ȴ���ȡֵ 404,ɾ�������
	uintptr_t				error;					// ִ�д���
	
	uintptr_t				target;					// ����Ŀ��
	task_block_parameter	parameter[20];			// ������
	uintptr_t				size;					// ��������

	uintptr_t				receive;				// ������
	uintptr_t				copy;					// �Ƿ���Ҫ��������
	uintptr_t				interrupt_level;		// ִ���жϵȼ�
	InternalServices* internal_services;			// ���÷����
}task_block;

EXTERN_C_END