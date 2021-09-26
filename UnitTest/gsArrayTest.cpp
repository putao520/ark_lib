#include "pch.h"
#include "CppUnitTest.h"
#include "../ark_sys/gs_array.h"
#include "../ark_sys/gs_array.c"
#include "../ark_sys/array_list.h"
#include "../ark_sys/array_list.c"
#include "../ark_sys/rtl.h"
#include "../ark_sys/rtl.c"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace gsArrayTest
{
	TEST_CLASS(gsArrayTest)
	{
	public:
		TEST_METHOD(TestArrayPointer) {
			array_list* list = NewArray();
			Assert::AreNotEqual((void *)list, (void *)NULL, L"新建 Array 对象为空");
			// 新增
			char* will_remove = NULL;
			for (int i = 0; i < 10; i++) {
				char* buff = (char *)_malloc(0x10);
				memset(buff, 0x56, 10);
				buff[9] = '\0';
				list->insert(list->handle, buff);
				if (i == 3)
					will_remove = buff;
			}

			// 删除
			if (will_remove)
				list->remove(list->handle, will_remove);

			// 遍历
			void* node = list->first(list->handle);
			while (node) {
				char* data = (char*)list->data(node);
				if (data) {
					printf("%s\n", data);
				}
				node = list->next(node);
			}
			DeleteArray(list);
		}

		TEST_METHOD(TestArrayValue) {
			array_list* list = NewArrayU64();
			Assert::AreNotEqual((void*)list, (void*)NULL, L"新建 Array 对象为空");
			// 新增
			uint64_t will_remove = NULL;
			for (uint64_t i = 0; i < 10; i++) {
				list->insert(list->handle, (void *)(i * 10000));
				if (i == 3)
					will_remove = i * 10000;
			}

			// 删除
			if (will_remove)
				list->remove(list->handle,(void *)will_remove);

			// 遍历
			void* node = list->first(list->handle);
			while (node) {
				uint64_t data = (uint64_t)list->data(node);
				if (data) {
					printf("%llx\n", data);
				}
				node = list->next(node);
			}
			DeleteArray(list);
		}
	};
}