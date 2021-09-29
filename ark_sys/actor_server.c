#include "actor_server.h"
#include "rtl.h"

unsigned int actor_cnt = 0;
// 最多支持20个参数
uintptr_t CallActorService(task_block* pTask) {
	// 错误地址校验
	if (!MmIsAddressValid((void *)pTask->target)) {
		pTask->error = TASK_ERROR_TARGET;
		return 0;
	}

	uintptr_t* parameter = &pTask->parameter;
	// 设置 中断等级
	KIRQL irql = KfRaiseIrql((KIRQL)pTask->interrupt_level);
	// 创建内核层参数中间块

	// 开始调用
	uintptr_t r = (uintptr_t)CallActorServiceGen(parameter, pTask->size, (void *)pTask->target);

	KeLowerIrql(irql);

	return r;
}

VOID ACTOR_SERVER_ROUTINE(actor_server* self) {
	// unsigned int idle_cnt = 0;

	if (!self)
		return;

	array_list* list = self->task_queue;
	if (!list)
		return;

	void* list_array = list->handle;

	// wait event 等待事件触发(触发事件 alerted 停止服务)
	PRKEVENT pkEvent = self->event_object;

	// 1秒超时
	// LARGE_INTEGER timeout = RtlConvertLongToLargeInteger(-10 * (1000*1000));

	while ( !NT_ERROR(KeWaitForSingleObject(pkEvent, Executive, KernelMode, FALSE, NULL)) ) {

		if (self->status == TASK_REMOVEING)
			break;
		
		// self->status = TASK_RUNNING;

		void* node = list->first(list_array);			
		while (node) {
			task_block* pTask = list->data(node);
			if (pTask) {
				switch (pTask->status) {
				case TASK_RUNNING:	// 运行中
					__debugbreak();
					pTask->receive = CallActorService(pTask);
					break;
				case TASK_REMOVEING:// 删除中（移除任务块时，需要判断 self->task_queue 是否为 remove 对象）
					list->remove_node(list_array, node);
					break;
				// default:			// 空闲中
					// idle_cnt++;
				}
				pTask->status = TASK_RETURNNING;
			}
			node = list->next(node);
		}
		
		// 设置为闲置对象
		// self->status = TASK_IDLE;
	}

	self->status = TASK_WAITTINGFREE;
	PsTerminateSystemThread(STATUS_SUCCESS);
}

actor_server* serve_fn(actor_server* self) {
	HANDLE hThread = NULL;
	PsCreateSystemThread(&hThread, 0, NULL, ZwCurrentProcess(), NULL, ACTOR_SERVER_ROUTINE, self);
	ZwClose(hThread);
	return self;
}

actor_server* accpet_fn(actor_server* self, task_block* block) {
	if (!self)
		return self;

#ifdef _DEBUG
	_printf("list:%p\n", self->task_queue);
	_printf("service:%p\n", self->internal_services);
#endif // _DEBUG

	array_list* list = self->task_queue;
	if (block->internal_services)
		memcpy(block->internal_services, self->internal_services, sizeof(InternalServices));

	if (list) {
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

	self->serve = TASK_IDLE;
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

	self->status = TASK_REMOVEING;
	KeSetEvent(self->event_object, IO_NO_INCREMENT, FALSE);
	while (self->status != TASK_WAITTINGFREE)
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