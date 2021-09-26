#pragma once
#ifdef WIN_KERNEL
#include <ntddk.h>
#else
#include <Windows.h>
#endif

#include "stdlib.h"

enum TASK_RUN_STATUS {
	IDLE,				// ������
	RUNNING,			// ������
	RETURNNING,			// ȡֵ��
	REMOVEING = 404		// ɾ���У��Ƴ������ʱ����Ҫ�ж� self->task_queue �Ƿ�Ϊ remove ����
}TASK_RUN_STATUS;

typedef struct task_block_parameter{
	uintptr_t	payload;
	uintptr_t	size;
	uintptr_t	opearte;	// 0 in, 1 out
}task_block_parameter;

typedef struct task_block {
	uintptr_t				status;					// ����״̬ 0������ 1,���ȴ���ִ�� 2,���ȴ���ȡֵ 404,ɾ�������
	
	uintptr_t				target;					// ����Ŀ��
	task_block_parameter	parameter[20];			// ������
	uintptr_t				size;				// ��������

	uintptr_t				receive;				// ������
	uintptr_t				copy;					// �Ƿ���Ҫ��������
}task_block;

typedef struct actor_server actor_server;

typedef actor_server* (*pfn_serve)(actor_server*);
typedef actor_server* (*pfn_accpet)(actor_server*, task_block* block);

typedef struct actor_server {
	array_list*		task_queue;		// ���������ָ��
	HANDLE			event_handle;	// �¼����
	pfn_serve		serve;			// ��ʼ����
	pfn_accpet		accpet;			// ͬ�����ע�����
	uintptr_t		status;			// 0 ���У� 1 ����
#ifdef WIN_KERNEL
	PKEVENT			event_object;
#else
	void* event_object;
#endif
}actor_server;