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