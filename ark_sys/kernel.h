#pragma once
#ifdef WIN_KERNEL
#include "Driver.h"
#endif

/*
typedef int(*read_internal_pfn)(void* buffer, void* address, int size);
typedef int(*write_internal_pfn)(void* buffer, void* address, int size);
*/

int read_internal_fn(void* buffer, void* address, int size);
int write_internal_fn(void* buffer, void* address, int size);

int process_read_internal_fn(void* eprocess, void* buffer, void* address, int size);
int process_write_internal_fn(void* eprocess, void* buffer, void* address, int size);

uintptr_t alloc_internal_fn(unsigned int size);
void free_internal_fn(void* address);

uintptr_t GetProcessPeb();