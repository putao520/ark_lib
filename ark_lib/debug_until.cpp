#include "debug_until.h"
#include "stdarg.h"
#include <iostream>
#include <Windows.h>

void __pauseDebug(const char* format, ...) {
	char info[256] = { 0 };

	va_list ptr;
	va_start(ptr, format);

	vsprintf_s(info, format, ptr);

	va_end(ptr);

	MessageBox(nullptr, info, "ต๗สิ", 16);
}

void printDebug(const char* format, ...) {
	char info[256] = { 0 };

	va_list ptr;
	va_start(ptr, format);

	vsprintf_s(info, format, ptr);

	va_end(ptr);

	printf("%s\n", info);

	MessageBox(nullptr, info, "ต๗สิ", 16);

}