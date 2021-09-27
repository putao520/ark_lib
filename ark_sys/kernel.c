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
		// ����Ŀ���ַ�������д���󲹣�
		memcpy(address, buffer, size);
		return size;
	}
	return -1;
}