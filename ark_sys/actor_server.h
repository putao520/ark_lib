#pragma once
#include "../ark_actor/actor_common.h"

EXTERN_C_START


typedef struct actor_server actor_server;

typedef actor_server* (*pfn_serve)(actor_server*);
typedef actor_server* (*pfn_accpet)(actor_server*, task_block* block);

typedef struct actor_server {
	array_list* task_queue;		// ���������ָ��
	HANDLE			event_handle;	// �¼����
	pfn_serve		serve;			// ��ʼ����
	pfn_accpet		accpet;			// ͬ�����ע�����
	uintptr_t		status;			// 0 ���У� 1 ����
#ifdef WIN_KERNEL
	PKEVENT			event_object;
#else
	void* event_object;
#endif
	InternalServices* internal_services;	// ���÷����
}actor_server;

void* CallActorServiceGen(task_block_parameter* parameter, uintptr_t size, void* target);
actor_server* NewActorServer(HANDLE eHandle);
void DeleteActorServer(actor_server* self);

EXTERN_C_END