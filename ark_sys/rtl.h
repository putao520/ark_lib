#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "stdarg.h"
#include "stdint.h"

#ifdef WIN_KERNEL
#include <ntddk.h>
#include <ntstrsafe.h>
#include "stdbool.h"
#else
#include <Windows.h>
#endif

#ifndef WIN_KERNEL
typedef short CSHORT;
typedef struct _TIME_FIELDS {
	CSHORT Year;        // range [1601...]
	CSHORT Month;       // range [1..12]
	CSHORT Day;         // range [1..31]
	CSHORT Hour;        // range [0..23]
	CSHORT Minute;      // range [0..59]
	CSHORT Second;      // range [0..59]
	CSHORT Milliseconds;// range [0..999]
	CSHORT Weekday;     // range [0..6] == [Sunday..Saturday]
} TIME_FIELDS;
typedef TIME_FIELDS* PTIME_FIELDS;
#endif

EXTERN_C_START

#define DELAY_ONE_MICROSECOND 	(-10)
#define DELAY_ONE_MILLISECOND	(DELAY_ONE_MICROSECOND*1000)

	void _printf(const char* _Format, ...);

	void _GetTickCount(PULONG msec);

	void _GetTickCount64(PULONGLONG msec);

	ULONG _GetCpuCount();
	ULONG _GetCpuNum();


	void _Sleep(unsigned long dwMilliseconds);
	int _sprintf(char* str, size_t size, const char* format, ...);

	void _QueryLocalTime(TIME_FIELDS* out);

	void* _malloc(size_t size);
	void _free(void* pointer);

	unsigned int _strlen(char* cs, unsigned int maxlength);
#ifdef _WIN64
	__int64 _atoi(const unsigned char* str);
	unsigned __int64 _atoui(const char* str);
	char* g_itoa(__int64 num, int radix);
	char* g_uitoa(unsigned __int64 num, int radix);
#else
	__int32 _atoi(const char* str);
	unsigned __int32 _atoui(const char* str);
	char* g_itoa(__int32 num, int radix);
	char* g_uitoa(unsigned __int32 num, int radix);

#endif

#ifndef WIN_KERNEL
	double _atof(const char* s);
#endif

EXTERN_C_END