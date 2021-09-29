#pragma once
typedef struct InternalServices{
	void* read;
	void* write;

	void* alloc;
	void* free;

	void* process_read;
	void* process_write;

	void* process_peb;
}InternalServices;

#ifdef WIN_KERNEL
InternalServices* NewInternalServices();
void DelInternalServices(InternalServices*);
#endif

