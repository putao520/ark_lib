#include "Driver.h"
#include "actor_server.h"

UNICODE_STRING DEVICE_NAME, LINK_NAME;
actor_server* global_actor_server = NULL;
VOID DriverUnload(PDRIVER_OBJECT pDriverObj) {
#ifdef _DEBUG
	_printf("DEVICE_QUIT\n");
#endif // _DEBUG
	IoDeleteSymbolicLink(&LINK_NAME);
	IoDeleteDevice(pDriverObj->DeviceObject);
}

NTSTATUS DispatchCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp) {
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS DispatchClose(PDEVICE_OBJECT pDevObj, PIRP pIrp) {
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp) {
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
	PIO_STACK_LOCATION pIrpStack;
	ULONG uIoControlCode;
	PVOID pIoBuffer;
	ULONG uInSize;
	ULONG uOutSize;

	pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
	uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;
	switch (uIoControlCode) {
		//在这里加入接口
	case IOCTL_IO_TEST:
#ifdef _DEBUG
		_printf("DEVICE_CONTROL_TEST\n");
#endif
		status = STATUS_SUCCESS;
		break;
	case IOCTL_IO_ACTOR_NEW: {
		// 只有一个 actor_server 实例
		uOutSize = sizeof(void*);
		if (!global_actor_server) {
			HANDLE eHandle = *(HANDLE *)pIoBuffer;
			global_actor_server = NewActorServer(eHandle);
		}
		*(void**)pIoBuffer = global_actor_server;
		status = STATUS_SUCCESS;
		break;
	}
	case IOCTL_IO_ACTOR_CONNECT:
		if (global_actor_server) {
			task_block* task = *(task_block**)pIoBuffer;
			global_actor_server->accpet(global_actor_server, task)
				->serve(global_actor_server);
			status = STATUS_SUCCESS;
		}
		else
			status = STATUS_UNSUCCESSFUL;
		break;
	default:
		break;
	}
	if (status == STATUS_SUCCESS)
		pIrp->IoStatus.Information = uOutSize;
	else
		pIrp->IoStatus.Information = 0;
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
#ifdef _DEBUG
	_printf("DEVICE_CONTROL\n");
#endif
	return status;
}

VOID InitDriverName(UNICODE_STRING* _DEVICE_NAME, UNICODE_STRING* _LINK_NAME, const wchar_t* driver_name) {
	RtlInitUnicodeString(_DEVICE_NAME, L"\\Device\\");
	RtlAppendUnicodeToString(_DEVICE_NAME, driver_name);
	RtlInitUnicodeString(_LINK_NAME, L"\\DosDevices\\");
	RtlAppendUnicodeToString(_LINK_NAME, driver_name);

#ifdef _DEBUG
	_printf("DEVICE_NAME:%ws\n", _DEVICE_NAME->Buffer);
	_printf("LINK_NAME:%ws\n", _LINK_NAME->Buffer);
#endif
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString) {
	// VMProtectBeginUltra("驱动入口");
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT pDevObj;
	//设置分发函数和卸载例程
	pDriverObj->Flags |= DO_BUFFERED_IO;

	pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
	pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;

	pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
	pDriverObj->DriverUnload = DriverUnload;

	//载入设备命名字符串
	InitDriverName(&DEVICE_NAME, &LINK_NAME, L"ArkSys");

	//创建一个设备
	status = IoCreateDevice(pDriverObj, 0, &DEVICE_NAME, FILE_DEVICE_UNKNOWN, 0, FALSE, &pDevObj);
	if (!NT_SUCCESS(status)) return status;
	//创建符号连接
	status = IoCreateSymbolicLink(&LINK_NAME, &DEVICE_NAME);
	if (!NT_SUCCESS(status)) {
		IoDeleteDevice(pDevObj);
		return status;
	}
	pDriverObj->DeviceObject = pDevObj;
#ifdef _DEBUG
	_printf("Code:%ws\n", DEVICE_NAME.Buffer);
	_printf("DEVICE_ENTRY\n");
	_printf("DEVICE_INFO:%ws\n", pDriverObj->HardwareDatabase->Buffer);
#endif
	return STATUS_SUCCESS;
	// VMProtectEnd();
}
