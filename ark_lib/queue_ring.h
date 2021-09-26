#pragma once
#include <utility>
#include "queue_ring_exception.h"

using namespace std;

template<typename T>
class queue_ring {
public:
	queue_ring();
	queue_ring(unsigned int);

	queue_ring<T>* push(T v) {
		buffer_ring[buffer_ring_max_index][behind_pos] = v;
		behind_pos++;
		if (behind_pos >= max_size) {
			buffer_ring_max_index++;

			// ���buffer�鳬��10��,����2����Сָ��
			if (buffer_ring_max_index >= buffer_block_max) {
				// ��ǰʹ��block�������Ƿ���ڵ�ǰ�趨���block������
				unsigned int len = buffer_ring_max_index - buffer_ring_min_index;
				if (len >= buffer_block_max) {
					buffer_block_max *= 2;
				}
				// ������ buffer_ring Array
				T** _buffer_ring = (T**)malloc( sizeof(T *) * buffer_block_max);
				for (unsigned int i = 0; i < len; i++) {
					_buffer_ring[i] = buffer_ring[buffer_ring_min_index + i];
				}
				buffer_ring = _buffer_ring;

				buffer_ring_max_index -= buffer_ring_min_index;
				buffer_ring_min_index = 0;
			}

			// ������ buffer 
			buffer_ring[buffer_ring_max_index] = (T*)malloc(sizeof(T) * max_size);
			// �����´� push λ��
			behind_pos = 0;
		}
		return this;
	}
	T&& pop() {

		if ( buffer_ring_min_index > buffer_ring_max_index ) {
			throw queue_ring_exception();
		}

		if ((buffer_ring_min_index == buffer_ring_max_index) && (front_pos == behind_pos)) {
			throw queue_ring_exception();
		}

		auto r = buffer_ring[buffer_ring_min_index][front_pos];
		front_pos++;
		if (front_pos >= max_size) {
			// ���ٵ�ǰ buffer(ָ���ͷ�BUG)
			if (buffer_ring_min_index < buffer_ring_max_index) {
				free( buffer_ring[buffer_ring_min_index] );
				buffer_ring_min_index++;
			}

			// �����´� pop λ��
			front_pos = 0;
		}

		return move(r);
	}
private:
	void init();

	unsigned int max_size = 1000;

	unsigned int front_pos;		// ��ͷ
	unsigned int behind_pos;	// ��β	

	unsigned int buffer_ring_max_index;
	unsigned int buffer_ring_min_index;

	unsigned int buffer_block_max;
	T** buffer_ring;
};