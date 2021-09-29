#include "print_until.h"
#include <stdio.h>

void printByteArray(char * pData, unsigned int size) {
	for (unsigned int i = 0; i < size; i++) {
		printf("0x%02x", pData[i]);
	}
	printf("\n");
}