#include "array_list.h"

/**
添加进list的data内存工作,我来接管
方便起见,data 如果是一个结构,结构内不要使用指针
*/

static void* chk_Handle(void* handle) {
	LIST* root = (LIST*)handle;
	return (root && root->root) ? handle : NULL;
}

void* _list(char dataSize, list_remove_callback remove_cb) {
	LIST* root = (LIST* )_malloc(sizeof(LIST));
	root->dataSize = dataSize;
	root->remove_cb = remove_cb;
	root->root = NULL;
	root->cursor = NULL;
	root->size = 0;
	// root->locker = Init_Lock();
	return root;
}

unsigned int get_data_size_list(void* handle) {
	if (!handle)
		return 0;
	LIST* root = (LIST*)handle;
	return root->dataSize;
}

void set_on_remove_node(void* handle, list_remove_callback remove_cb) {
	if (!handle)
		return;
	LIST* root = (LIST*)handle;
	root->remove_cb = remove_cb;
}

void del_list(void* handle) {
	if (!handle)
		return;
	// LIST* root = (LIST*)handle;
	// lock(&root->locker);

	void* node = first_list_node(handle);
	// int i = 0;
	while (node) {
		/*
		i++;
		_printf("删除节点[%d]:%#llx 数据::%#llx\n", i, node, data_list_node(node));
		*/
		node = remove_list_node(handle, node);
	}
	// unlock(&root->locker);
	// 最后释放列表结构
	_free(handle);
}
// 当 rootNode为 NULL时,表示插在头部
static void insert_last_node(LIST* root, LIST_NODE* rootNode, LIST_NODE* newNode) {
	root->size++;
	newNode->prev = rootNode;
	// 附加节点为空,设置为列表尾节点
	if (!rootNode) {
		newNode->next = root->root;
		root->root = newNode;
	}
	else {
		newNode->next = rootNode->next;
		rootNode->next = newNode;
	}

	if (newNode->next)
		newNode->next->prev = newNode;
	else
		root->cursor = newNode;
}

static void insert_node(LIST* root, LIST_NODE* rootNode, LIST_NODE* newNode) {
	root->size++;
	// 附加节点为空,设置为列表尾节点
	if (!rootNode)
		rootNode = root->cursor;

	// 设置上一个节点为附加节点
	newNode->prev = rootNode;

	// 附加节点和列表尾节点都不为空
	if (rootNode) {
		newNode->next = rootNode->next;
		if (rootNode->next)
			rootNode->next->prev = newNode;
		rootNode->next = newNode;
	}

	if (!root->root)
		root->root = newNode;

	root->cursor = newNode;
}

// 插入数据data到node后面
void* insert_ex_list_node(void* handle, void* data, void* node) {
	if (!handle || !data)
		return NULL;
	LIST* root = (LIST*)handle;
	LIST_NODE* newNode = (LIST_NODE*)_malloc(sizeof(LIST_NODE));

	if (!newNode)
		return NULL;
	newNode->data = data;
	newNode->prev = NULL;
	newNode->next = NULL;
	insert_node(root, (LIST_NODE*)node, newNode);
	return newNode;
}

// 附加节点为空时,将数据插入到链头
void* insert_ex_last_list_node(void* handle, void* data, void* node) {
	if (!handle || !data)
		return NULL;
	LIST* root = (LIST*)handle;
	LIST_NODE* newNode = (LIST_NODE* )_malloc(sizeof(LIST_NODE));

	if (!newNode)
		return NULL;
	newNode->data = data;
	newNode->prev = NULL;
	newNode->next = NULL;
	// newNode->backup1 = 0xeeeeeeee;
	// newNode->backup1 = 0xaaaaaaaa;
	insert_last_node(root, (LIST_NODE*)node, newNode);

	return newNode;
}

void* insert_list_node(void* handle, void* data) {
	if (!data)
		return NULL;
	// LIST* root = (LIST*)handle;
	// lock(&root->locker);
	void* r = insert_ex_list_node(handle, data, NULL);
	// unlock(&root->locker);
	return r;
}

void* push_list_node(void* handle, void* node) {
	if (!handle || !node)
		return NULL;
	LIST* root = (LIST*)handle;
	// lock(&root->locker);
	node = reset_node_list(node);

	insert_node(root, NULL, (LIST_NODE*)node);
	// unlock(&root->locker);
	return node;
}

void* find_list_node(void* handle, void* data) {
	void* node = first_list_node(handle);
	while (node) {
		if (data == data_list_node(node))
			return node;
		node = next_list_node(node);
	}
	return NULL;
}

void each_list_node(void* handle, list_each_callback cb) {
	if (!chk_Handle(handle) || !cb)
		return;
	LIST* root = (LIST*)handle;
	for (LIST_NODE* node = root->root;
		node != NULL;
		node = node->next) {
		cb(node->data);
	}
}

static int compare(void* data1, void* data2, unsigned int len) {

	switch (len) {
	case 1: {
		unsigned char d1 = (unsigned char)data1, d2 = (unsigned char)data2;
		return d1 - d2;
	}
	case 2: {
		unsigned short d1 = (unsigned short)data1, d2 = (unsigned short)data2;
		return d1 - d2;
	}
	case 4: {
		unsigned __int32 d1 = (unsigned __int32)data1, d2 = (unsigned __int32)data2;
		return d1 - d2;
	}
	case 8: {
		unsigned __int64 d1 = (unsigned __int64)data1, d2 = (unsigned __int64)data2;
		return (int)(d1 - d2);
	}
	default: {
		int k1len = _strlen((char *)data1, 255), k2len = _strlen((char *)data2, 255);
		return k1len == k2len ? memcmp(data1, data2, k1len) : k1len - k2len;
	}
	}
}

int del_list_node(void* handle, void* data) {
	int i = 0;
	if (!chk_Handle(handle) || !data)
		return -1;
	LIST* root = (LIST*)handle;
	// lock(&root->locker);
	for (LIST_NODE* node = root->root;
		node != NULL;
		node = node->next) {
		// 发现值相等的节点
		if (compare((char*)data, (char*)node->data, root->dataSize) == 0) {
			node = (LIST_NODE*)remove_list_node(handle, node);
		}
	}
	// unlock(&root->locker);
	return i;
}

void* first_list_node(void* handle) {
	if (!chk_Handle(handle))
		return NULL;
	LIST* root = (LIST*)handle;
	return root->root;
}

void* next_list_node(void* node) {
	if (!node)
		return NULL;
	LIST_NODE* _node = (LIST_NODE*)node;
	return _node->next;
}

void* last_list_node(void* handle) {
	if (!chk_Handle(handle))
		return NULL;
	LIST* root = (LIST*)handle;
	return root->cursor;
}

void* prev_list_node(void* node) {
	if (!node)
		return NULL;
	LIST_NODE* _node = (LIST_NODE*)node;
	return _node->prev;
}
void* pop_next_list_node(void* handle, void* node) {
	if (!chk_Handle(handle) || !node)
		return NULL;
	LIST* root = (LIST*)handle;
	root->size--;

	LIST_NODE* _node = (LIST_NODE*)node;

	LIST_NODE* nextNode = _node->next;
	LIST_NODE* prevNode = _node->prev;
	if (prevNode)
		prevNode->next = nextNode;
	else
		root->root = nextNode;

	if (nextNode)
		nextNode->prev = prevNode;
	else
		root->cursor = prevNode;

	return nextNode;
}
/**
返回当前删除节点的上一个节点
*/
void* remove_list_node(void* handle, void* node) {
	LIST* root = (LIST*)handle;
	if (!root)
		return NULL;

	LIST_NODE* nextNode = (LIST_NODE*)pop_next_list_node(handle, node);

	if (root && root->remove_cb && node) {
		root->remove_cb(data_list_node(node));
	}

	_free(node);

	return nextNode;
}

/**
返回当前删除节点的上一个节点(跳过回收函数)
*/
void* removeImpl_list_node(void* handle, void* node) {
	LIST* root = (LIST*)handle;
	if (!root)
		return NULL;

	LIST_NODE* nextNode = (LIST_NODE*)pop_next_list_node(handle, node);

	_free(node);

	return nextNode;
}

void* data_list_node(void* node) {
	LIST_NODE* _node = (LIST_NODE*)node;
	return _node->data;
}

unsigned int get_size_list(void* handle) {
	if (!handle)
		return 0;
	LIST* root = (LIST*)handle;
	return root->size;
}

void* reset_root_node_list(void* handle) {
	if (!handle)
		return 0;
	LIST* root = (LIST*)handle;
	root->size = 0;
	root->root = NULL;
	root->cursor = NULL;
	return handle;
}

void* reset_node_list(void* node) {
	LIST_NODE* _node = (LIST_NODE*)node;
	_node->prev = NULL;
	_node->next = NULL;
	return _node;
}
