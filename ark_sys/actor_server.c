#include "actor_server.h"
#include "rtl.h"

unsigned int actor_cnt = 0;

void SwapInputParameter(task_block_parameter* dst_parameter, task_block_parameter* src_parameter, uintptr_t size) {
	for (int i = 0; i < size; i++) {
		memcpy(dst_parameter, src_parameter, sizeof(task_block_parameter));
		if (src_parameter[i].size ) {
			void* buffer = _malloc(src_parameter[i].size);
			if (buffer) {
				memcpy(buffer, (void*)src_parameter[i].payload, src_parameter[i].size);
			}
			dst_parameter[i].payload = buffer;
		}
	}
}

void SwapOutputParameter(task_block_parameter* dst_parameter, task_block_parameter* src_parameter, uintptr_t size) {
	for (int i = 0; i < size; i++) {
		// 是一个输出接收参数
		if (src_parameter[i].size && src_parameter[i].opearte == 1) {
			memcpy((void *)dst_parameter[i].payload, (void*)src_parameter[i].payload, dst_parameter[i].size);
		}
	}
}

// 最多支持20个参数
uintptr_t CallActorService(task_block* pTask) {
	task_block_parameter* new_parameter = NULL;
	task_block_parameter* parameter = pTask->parameter;
	// 创建内核层参数中间块
	if (pTask->copy) {
		new_parameter = _malloc( sizeof(task_block_parameter) * 20 );
		if (new_parameter) {
			SwapInputParameter(new_parameter, parameter, pTask->size);
		}
		parameter = new_parameter;
	}
	// 开始调用
	uintptr_t r = CallActorServiceGen(parameter, pTask->size, (void *)pTask->target);
	if (pTask->copy && new_parameter) {
		SwapOutputParameter(parameter, new_parameter, pTask->size);
	}
	return r;
}

VOID ACTOR_SERVER_ROUTINE(actor_server* self) {

	__debugbreak();

	unsigned int idle_cnt = 0;

	if (!self)
		return;

	array_list* list = self->task_queue;
	if (!list)
		return;

	void* list_array = list->handle;

	// wait event 等待事件触发(触发事件 alerted 停止服务)
	PRKEVENT pkEvent = self->event_object;

	while ( !NT_ERROR(KeWaitForSingleObject(pkEvent, Executive, KernelMode, FALSE, NULL)) ) {
		if (self->status == REMOVEING)
			break;
		
		if (idle_cnt > 10000) {
			self->status = IDLE;
			KeClearEvent(pkEvent);
		}
		else
			self->status = RUNNING;

		void* node = list->first(list_array);
		while (node) {
			task_block* pTask = list->data(node);
			if (pTask) {
				switch (pTask->status) {
				case RUNNING:	// 运行中
					pTask->receive = CallActorService(pTask);
					pTask->status = RETURNNING;
					idle_cnt = 0;
					break;
				case REMOVEING: // 删除中（移除任务块时，需要判断 self->task_queue 是否为 remove 对象）
					list->remove_node(list_array, node);
					break;
				default:		// 空闲中
					idle_cnt++;
				}
			}
			node = list->next(node);
		}
	}

	self->status = WAITTINGFREE;
	PsTerminateSystemThread(STATUS_SUCCESS);
}

actor_server* serve_fn(actor_server* self) {
	HANDLE hThread = NULL;
	PsCreateSystemThread(&hThread, 0, NULL, ZwCurrentProcess(), NULL, ACTOR_SERVER_ROUTINE, self);
	ZwClose(hThread);
	return self;
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
#ifdef _WIN64
	self->task_queue = NewArrayU64();
#else
	self->task_queue = NewArrayU32();
#endif

	self->accpet = accpet_fn;
	self->serve = serve_fn;

	self->event_handle = eHandle;
	if (NT_SUCCESS(ObReferenceObjectByHandle(eHandle, EVENT_ALL_ACCESS, *ExEventObjectType, KernelMode, &self->event_object, NULL))) {
		// ObDereferenceObject(self->event_object);
	}

	self->internal_services = NewInternalServices();

	actor_cnt++;

	return self;
}

void DeleteActorServer(actor_server* self) {
	if (!self)
		return;

	self->status = REMOVEING;
	KeSetEvent(self->event_object, IO_NO_INCREMENT, FALSE);
	while (self->status != WAITTINGFREE)
		_Sleep(1000);

	if (self->task_queue)
		DeleteArray(self->task_queue);

	if (self->event_handle)
		ZwClose(self->event_handle);

	if (self->event_object)
		ObDereferenceObject(self->event_object);
		
	if (self->internal_services)
		DelInternalServices(self->internal_services);

	_free(self);
}