#include "actor_server.h"

unsigned int actor_cnt = 0;

void SwapInputParameter(task_block_parameter* dst_parameter, task_block_parameter* src_parameter, uintptr_t size) {
	for (int i = 0; i < size; i++) {
		memcpy(dst_parameter, src_parameter, sizeof(task_block_parameter));
		if (src_parameter[i].size ) {
			void* buffer = _malloc(src_parameter[i].size);
			if (buffer) {
				memcpy(buffer, src_parameter[i].payload, src_parameter[i].size);
			}
			dst_parameter[i].payload = buffer;
		}
	}
}

void SwapOutputParameter(task_block_parameter* dst_parameter, task_block_parameter* src_parameter, uintptr_t size) {
	for (int i = 0; i < size; i++) {
		// ��һ��������ղ���
		if (src_parameter[i].size && src_parameter[i].opearte == 1) {
			memcpy(dst_parameter[i].payload, src_parameter[i].payload, dst_parameter[i].size);
		}
	}
}

// ���֧��20������
uintptr_t CallActorService(task_block* pTask) {
	task_block_parameter* new_parameter = NULL;
	task_block_parameter* parameter = pTask->parameter;
	// �����ں˲�����м��
	if (pTask->copy) {
		task_block_parameter* new_parameter = _malloc( sizeof(task_block_parameter) * 20 );
		if (new_parameter) {
			SwapInputParameter(new_parameter, parameter, pTask->size);
		}
		parameter = new_parameter;
	}
	// ��ʼ����
	uintptr_t r = CallActorServiceGen(parameter, pTask->size, pTask->target);
	if (pTask->copy && new_parameter) {
		SwapOutputParameter(parameter, new_parameter, pTask->size);
	}
	return r;
}

VOID ACTOR_SERVER_ROUTINE(actor_server* self) {
	NTSTATUS status;
	unsigned int idle_cnt = 0;

	if (!self)
		return;

	array_list* list = self->task_queue;
	if (!list)
		return;

	void* list_array = list->handle;

	self->status = RUNNING;

	// wait event �ȴ��¼�����(�����¼� alerted ֹͣ����)
	PRKEVENT pkEvent = self->event_object;

	while ( !NT_ERROR(KeWaitForSingleObject(pkEvent, Executive, KernelMode, FALSE, NULL)) ) {

		if (idle_cnt > 10000) {
			self->status = IDLE;
			_sleep(100);
		}

		void* node = list->first(list_array);
		while (node) {
			task_block* pTask = list->data(node);
			if (pTask) {
				switch (pTask->status) {
				case RUNNING:	// ������
					pTask->receive = CallActorService(pTask);
					pTask->status = RETURNNING;
					idle_cnt = 0;
					break;
				case REMOVEING: // ɾ���У��Ƴ������ʱ����Ҫ�ж� self->task_queue �Ƿ�Ϊ remove ����
					list->remove_node(list_array, node);
					break;
				default:		// ������
					idle_cnt++;
				}
			}
			node = list->next(node);
		}
	}

	PsTerminateSystemThread(STATUS_SUCCESS);
}

actor_server* serve_fn(actor_server* self) {
	HANDLE hThread = NULL;
	PsCreateSystemThread(&hThread, 0, NULL, NULL, NULL, ACTOR_SERVER_ROUTINE, self);
	ZwClose(hThread);
}

actor_server* accpet_fn(actor_server* self, task_block* block) {
	array_list* list = self->task_queue;
	if (self) {
		list->insert(list->handle, block);
	}
	return self;
}

actor_server* NewActorServer(HANDLE eHandle) {
	if (!eHandle)
		return NULL;

	actor_server* self = _malloc(sizeof(actor_server));
	if (!self)
		return self;

	self->serve = IDLE;
	self->task_queue = NewArray();

	self->accpet = accpet_fn;
	self->serve = serve_fn;

	self->event_handle = eHandle;
	if (NT_SUCCESS(ObReferenceObjectByHandle(eHandle, EVENT_MODIFY_STATE, *ExEventObjectType, KernelMode, self->event_object, NULL))) {
		ObDereferenceObject(self->event_object);
	}
	
	self->event_object = _malloc(sizeof(KEVENT));
	actor_cnt++;

	return self;
}

void DeleteActorServer(actor_server* self) {
	if (!self)
		return;

	if (self->task_queue)
		DeleteArray(self->task_queue);

	if (self->event_handle)
		ZwClose(self->event_handle);

	if (self->event_object)
		_free(self->event_object);

	_free(self);
}