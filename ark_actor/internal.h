#pragma once

#include "../ark_sys/kernel.h"

typedef struct InternalServices{
	read_internal_pfn read;
	write_internal_pfn write;
}InternalServices;

#ifdef WIN_KERNEL
InternalServices* NewInternalServices();
void DelInternalServices(InternalServices*);
#endif

