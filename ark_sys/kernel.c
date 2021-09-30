#include "kernel.h"

int read_internal_fn(void* buffer, void* address, int size) {
	if (MmIsAddressValid(buffer) && MmIsAddressValid(address) && MmIsAddressValid((char*)address + size - 1)) {
		memcpy(buffer, address, size);
		return size;
	}	
	return -1;
}

int write_internal_fn(void* buffer, void* address, int size) {
	if (MmIsAddressValid(buffer) && MmIsAddressValid(address) && MmIsAddressValid((char*)address + size - 1)) {
		// 设置目标地址必须可以写（后补）
		memcpy(address, buffer, size);
		return size;
	}
	return -1;
}

uintptr_t alloc_internal_fn(unsigned int size) {
	return (uintptr_t)_malloc(size);
}

void free_internal_fn(void* address) {
	if (MmIsAddressValid(address))
		_free(address);
}

int inline process_read_internal_impl_fn(void* eprocess, void* buffer, void* address, int size) {
	KAPC_STATE apcState = { 0 };
	KeStackAttachProcess(eprocess, &apcState);

	read_internal_fn(buffer, address, size);

	KeUnstackDetachProcess(&apcState);

	return size;
}

int process_read_internal_fn(void* eprocess, void* buffer, void* address, int size) {
	if (IoGetCurrentProcess() == eprocess) {
		process_read_internal_impl_fn(eprocess, buffer, address, size);
	}
	else {
		char swapBuffer[256] = { 0 };
		if (size > 256) {
			void* _buffer = _malloc(size);
			if (_buffer) {
				process_read_internal_impl_fn(eprocess, _buffer, address, size);
				read_internal_fn(buffer, _buffer, size);
				_free(_buffer);
			}
		}
		else {
			void* _buffer = swapBuffer;
			process_read_internal_impl_fn(eprocess, _buffer, address, size);
			read_internal_fn(buffer, _buffer, size);
		}
	}
	return size;
}

int inline process_write_internal_impl_fn(void* eprocess, void* buffer, void* address, int size) {
	KAPC_STATE apcState = { 0 };
	KeStackAttachProcess(eprocess, &apcState);

	write_internal_fn(buffer, address, size);

	KeUnstackDetachProcess(&apcState);

	return size;
}

int process_write_internal_fn(void* eprocess, void* buffer, void* address, int size) {
	if (IoGetCurrentProcess() == eprocess) {
		process_write_internal_fn(eprocess, buffer, address, size);
	}
	else {
		char swapBuffer[256] = { 0 };
		if (size > 256) {
			void* _buffer = _malloc(size);
			if (_buffer) {
				process_write_internal_impl_fn(eprocess, _buffer, address, size);
				write_internal_fn(buffer, _buffer, size);
				_free(_buffer);
			}
		}
		else {
			void* _buffer = swapBuffer;
			process_write_internal_impl_fn(eprocess, _buffer, address, size);
			write_internal_fn(buffer, _buffer, size);
		}
	}
	return size;
}

unsigned int str_bufferlen_fn(char* buffer) {
	char* start = buffer;
	while (MmIsAddressValid(buffer) && *buffer)
		buffer++;

	return buffer - start;
}

unsigned int ws_bufferlen_fn(short* buffer) {
	short* start = buffer;
	while (MmIsAddressValid(buffer) && *buffer)
		buffer++;

	return buffer - start;
}