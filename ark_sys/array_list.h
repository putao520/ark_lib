#pragma once
#include "rtl.h"

EXTERN_C_START

	// 卸载回调
	// p: 内容值
	typedef void (*list_remove_callback)(void* data);

	typedef struct LIST_NODE LIST_NODE;

	typedef struct LIST_NODE {
		// unsigned int backup1;
		void* data;
		struct LIST_NODE* next;
		struct LIST_NODE* prev;
		// unsigned int backup2;
	}LIST_NODE;

	typedef struct LIST {
		// 内容最大长度
		unsigned int dataSize;
		// 内容数量
		unsigned int size;

		list_remove_callback remove_cb;
		// 第一个节点指针
		struct LIST_NODE* root;
		// 最后一个节点指针
		struct LIST_NODE* cursor;

		// pLocker locker;
	}LIST, * pLIST;

#define LIST_BYTE 1
#define LIST_SHORT 2
#define LIST_DWORD 4
#define LIST_QWORD 8
#ifdef _WIN64
#define LIST_POINT 8
#else
#define LIST_POINT 4
#endif // _WIN64
#define LIST_STRING -1

	// 遍历回调
	// p: 内容值
	typedef void (*list_each_callback)(void* data);

	// 创建新的数组
	// p: 内容最大长度
	void* _list(char dataSize, list_remove_callback remove_cb);
	// 获得内容最大长度
	unsigned int get_data_size_list(void* handle);
	// 获得列表总量
	unsigned int get_size_list(void* handle);

	void* find_list_node(void* handle, void* data);
	// 释放数组
	// p: 数组句柄
	void del_list(void* handle);

	// 移除某个内容
	// p: 数组句柄
	// p: 内容指针
	int del_list_node(void* handle, void* data);

	void* pop_next_list_node(void* handle, void* node);
	// 新增一个内容
	// p: 数组句柄
	// p: 内容指针
	void* insert_list_node(void* handle, void* data);
	void* insert_ex_list_node(void* handle, void* data, void* node);
	void* insert_ex_last_list_node(void* handle, void* data, void* node);
	// 添加一个list节点
	void* push_list_node(void* handle, void* node);
	// 遍历每一个内容
	// p: 数组句柄
	// p: 内容指针
	void each_list_node(void* handle, list_each_callback cb);
	void* reset_root_node_list(void* handle);
	void set_on_remove_node(void* handle, list_remove_callback remove_cb);

	void* first_list_node(void* handle);
	void* last_list_node(void* handle);

	void* next_list_node(void* node);
	void* prev_list_node(void* node);

	void* remove_list_node(void* handle, void* node);
	void* removeImpl_list_node(void* handle, void* node);
	void* data_list_node(void* node);

	void* reset_node_list(void* node);

EXTERN_C_END
