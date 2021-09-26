#pragma once
#include "zzzJson.h"
#include "rtl.h"
typedef struct zzz_Value zzz_Value;
typedef struct zzz_Allocator zzz_Allocator;

typedef struct Json Json, * pJson;

typedef struct JsonArray JsonArray, * pJsonArray;

typedef struct JsonAllocater {
	zzz_Allocator* allocater;
	volatile unsigned int ref_cnt;
}JsonAllocater, * pJsonAllocater;

EXTERN_C_START

	int InitJsonAllocater(JsonAllocater** v);
	int DestoryJsonAllocater(JsonAllocater** v);
	void UpdateJsonAllocater(JsonAllocater* p);

	zzz_Value* zzz_ValueCopyAuto(zzz_Value* dest, zzz_Value* src);

EXTERN_C_END
