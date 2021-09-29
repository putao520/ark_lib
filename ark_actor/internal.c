#include "internal.h"
#include "../ark_sys/rtl.h"
#include "../ark_sys/kernel.h"

InternalServices* NewInternalServices() {
	InternalServices* self = _malloc(sizeof(InternalServices));
	if (!self)
		return 0;

	self->read = (void*)read_internal_fn;
	self->write = (void*)write_internal_fn;

	self->process_read = (void*)process_read_internal_fn;
	self->process_write = (void*)process_write_internal_fn;

	self->process_peb = (void*)GetProcessPeb;
	return self;
}

void DelInternalServices(InternalServices* self) {
	if (!self)
		return;
	_free(self);
}