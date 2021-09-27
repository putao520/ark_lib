#include "internal.h"
#include "../ark_sys/rtl.h"

InternalServices* NewInternalServices() {
	InternalServices* self = _malloc(sizeof(InternalServices));
	if (!self)
		return 0;

	self->read = read_internal_fn;
	self->write = write_internal_fn;

	return self;
}

void DelInternalServices(InternalServices* self) {
	if (!self)
		return;
	_free(self);
}