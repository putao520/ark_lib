#pragma once
#include "array_list.h"

typedef unsigned int (*pfn_size)(void* handle);
typedef unsigned int (*pfn_remove)(void* handle, void* data);
typedef void* (*pfn_insert)(void* handle, void* data);
typedef void (*pfn_on_remove)(void* handle, list_remove_callback remove_cb);

typedef void* (*pfn_first)(void* handle);
typedef void* (*pfn_next)(void* node);
typedef void* (*pfn_data)(void* node);
typedef void* (*pfn_remove_node)(void* handle, void* node);

typedef struct array_list{
	pfn_insert insert;
	pfn_remove remove;
	pfn_size size;
	pfn_on_remove on_remove;

	pfn_first first;
	pfn_next next;
	pfn_data data;
	pfn_remove_node remove_node;

	void* handle;
}array_list;

array_list* NewArray();	// ÷∏’Î
array_list* NewArrayByte();
array_list* NewArrayU32();
array_list* NewArrayU64();

void DeleteArray(array_list*);