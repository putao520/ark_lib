#include "gs_array.h"

array_list* NewArrayImpl(unsigned int data_size, list_remove_callback cb) {
	array_list* self = (array_list*)_malloc(sizeof(array_list));
	if (self) {
		self->handle = _list((char)data_size, NULL);

		self->insert = insert_list_node;
		self->remove = del_list_node;
		self->size = get_size_list;
		self->on_remove = set_on_remove_node;

		self->first = first_list_node;
		self->next = next_list_node;
		self->data = data_list_node;
		self->remove_node = remove_list_node;

		if (cb)
			set_on_remove_node(self->handle, cb);
	}
	return self;
}

void DeleteArray(array_list* handle) {
	if (!handle)
		return;

	if (handle->handle) {
		del_list(handle->handle);
	}

	_free(handle);
}

array_list* NewArrayByte() {
	return NewArrayImpl(LIST_BYTE, NULL);
}

array_list* NewArrayU32() {
	return NewArrayImpl(LIST_DWORD, NULL);
}

array_list* NewArrayU64() {
	return NewArrayImpl(LIST_QWORD, NULL);
}

void default_point_remove(void* pointer) {
	if (pointer)
		_free(pointer);
}

array_list* NewArray() {
	return NewArrayImpl(LIST_POINT, default_point_remove);
}