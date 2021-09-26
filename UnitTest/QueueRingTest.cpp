#include "pch.h"
#include "CppUnitTest.h"
#include "../ark_lib/queue_ring.h"
#include "../ark_lib/queue_ring.cpp"
#include "../ark_lib/queue_ring_exception.h"
#include "../ark_lib/queue_ring_exception.cpp"
#include "ClassDemo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace QueueRingTest
{
	TEST_CLASS(QueueRingTest)
	{
	public:
		
		TEST_METHOD(TestQueueRing)
		{
			queue_ring<ClassDemo<unsigned int>> queue(2);

			// ����:��Ϊ��� block ��Ϊ 5 ��ÿ����2������
			// ����:������
			for (unsigned int i = 0; i < 10; i++) {
				ClassDemo<unsigned int> v(i);
				queue.push(v);
			}

			// ����:�鸴��
			ClassDemo<unsigned int> v(10);
			queue.push(v);

			// ����:�����
			for (unsigned int i = 0; i < 11; i++) {
				ClassDemo<unsigned int> v = queue.pop();
				printf("[%d]->%d\n", i, v.getValue());
				Assert::AreEqual(v.getValue(), i);
			}
			try {
				// ����:��������
				queue.pop();
			}
			catch (queue_ring_exception& e) {
				const char* info = e.what();
				Assert::AreEqual(info, "queue pop overflow");
			}

		}
	};
}
