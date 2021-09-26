#pragma once
#include "json_common.h"
#include "json_array_iter.h"

EXTERN_C_START

	typedef pJsonArray(*def_json_array_fromString)(pJsonArray self, const char* json_string);
	typedef pJsonArray(*def_json_array_fromJson)(pJsonArray self, Json* json_object);
	typedef pJsonArray(*def_json_array_fromJsonArray)(pJsonArray self, JsonArray* json_object);
	typedef int (*def_json_array_getSize)(pJsonArray self);
	typedef BOOL(*def_json_array_isNull)(pJsonArray self);
	typedef BOOL(*def_json_array_isObject)(pJsonArray self);
	typedef const char* (*def_json_array_toSting)(pJsonArray self);
	typedef pJson(*def_json_array_toJson)(pJsonArray self);
	typedef pJsonArray(*def_json_array_clear)(pJsonArray self);
	typedef pJsonArray(*def_json_array_remove)(pJsonArray self, unsigned int idx);
	typedef pJsonArray(*def_json_array_addJsonArray)(pJsonArray self, JsonArray* json_object);
	typedef pJsonArray(*def_json_array_destory)(pJsonArray self);

	typedef const char* (*def_json_array_getString)(pJsonArray self, unsigned int idx);
	typedef const char* (*def_json_array_getUnEscapeString)(pJsonArray self, unsigned int idx);
	typedef zzz_JSONType(*def_json_array_getType)(pJsonArray self, unsigned int idx);
	typedef int (*def_json_array_getInt)(pJsonArray self, unsigned int idx);
	typedef long (*def_json_array_getLong)(pJsonArray self, unsigned int idx);
	typedef long long (*def_json_array_getLL)(pJsonArray self, unsigned int idx);
	typedef BOOL(*def_json_array_getBool)(pJsonArray self, unsigned int idx);
	typedef JsonArray* (*def_json_array_getArray)(pJsonArray self, unsigned int idx);
	typedef Json* (*def_json_array_getJson)(pJsonArray self, unsigned int idx);
	typedef double (*def_json_array_getDouble)(pJsonArray self, unsigned int idx);

	typedef JsonArray* (*def_json_array_replaceString)(pJsonArray self, unsigned int idx, const char* value);
	typedef JsonArray* (*def_json_array_replaceEscapeString)(pJsonArray self, unsigned int idx, const char* value);
	typedef JsonArray* (*def_json_array_replaceInt)(pJsonArray self, unsigned int idx, int value);
	typedef JsonArray* (*def_json_array_replaceLong)(pJsonArray self, unsigned int idx, long value);
	typedef JsonArray* (*def_json_array_replaceLL)(pJsonArray self, unsigned int idx, long long value);
	typedef JsonArray* (*def_json_array_replaceBool)(pJsonArray self, unsigned int idx, BOOL value);
	typedef JsonArray* (*def_json_array_replaceArray)(pJsonArray self, unsigned int idx, JsonArray* value);
	typedef JsonArray* (*def_json_array_replaceJson)(pJsonArray self, unsigned int idx, Json* value);
	typedef JsonArray* (*def_json_array_replaceDouble)(pJsonArray self, unsigned int idx, double value);

	typedef int (*def_json_array_putString)(pJsonArray self, const char* value);
	typedef int (*def_json_array_putEscapeString)(pJsonArray self, const char* value);
	typedef int (*def_json_array_putInt)(pJsonArray self, int value);
	typedef int (*def_json_array_putLong)(pJsonArray self, long value);
	typedef int (*def_json_array_putLL)(pJsonArray self, long long value);
	typedef int (*def_json_array_putBool)(pJsonArray self, BOOL value);
	typedef int (*def_json_array_putArray)(pJsonArray self, JsonArray* value);
	typedef int (*def_json_array_putJson)(pJsonArray self, Json* value);
	typedef int (*def_json_array_putDouble)(pJsonArray self, double value);

	typedef JsonArrayIter* (*def_json_array_iter_first)(pJsonArray self);

	typedef zzz_BOOL def_set_common(zzz_Value* o, void* v);
	typedef zzz_BOOL def_set_common8B(zzz_Value* o, double v);

	typedef struct CacheIdxJson {
		unsigned int tmp_idx;
		zzz_Value* tmp_obj;
	}CacheIdxJson;

	typedef struct JsonArray {
		JsonAllocater* allocater;
		zzz_Value* glb_obj;
		CacheIdxJson cache;
		zzz_BOOL error_status;
		JsonArrayIter* iter;
		const char* string_data;

		def_json_array_fromString fromString;
		def_json_array_fromJson fromJson;
		def_json_array_fromJsonArray fromJsonArray;
		def_json_array_getSize getSize;
		def_json_array_isNull isNull;
		def_json_array_isObject isObject;
		def_json_array_toSting toString;
		def_json_array_addJsonArray addJsonArray;
		def_json_array_getType getType;
		def_json_array_toJson toJson;
		def_json_array_destory destory;

		def_json_array_getString getString;
		def_json_array_getUnEscapeString getUnEscapeString;
		def_json_array_getInt getInt;
		def_json_array_getLong getLong;
		def_json_array_getLL getLL;
		def_json_array_getBool getBool;
		def_json_array_getArray getArray;
		def_json_array_getJson getJson;
		def_json_array_getDouble getDouble;

		def_json_array_replaceString replaceString;
		def_json_array_replaceEscapeString replaceEscapeString;
		def_json_array_replaceInt replaceInt;
		def_json_array_replaceLong replaceLong;
		def_json_array_replaceLL replaceLL;
		def_json_array_replaceBool replaceBool;
		def_json_array_replaceArray replaceArray;
		def_json_array_replaceJson replaceJson;
		def_json_array_replaceDouble replaceDouble;

		def_json_array_putString putString;
		def_json_array_putEscapeString putEscapeString;
		def_json_array_putInt putInt;
		def_json_array_putLong putLong;
		def_json_array_putLL putLL;
		def_json_array_putBool putBool;
		def_json_array_putArray putArray;
		def_json_array_putJson putJson;
		def_json_array_putDouble putDouble;

		def_json_array_remove remove;
		def_json_array_clear clear;

		def_json_array_iter_first first;
	}JsonArray, * pJsonArray;

	JsonArray* InitJsonArrayWrap(JsonAllocater* allocater, zzz_Value* v);
	JsonArray* InitJsonArray();
	void DestoryJsonArray(JsonArray* self);

EXTERN_C_END
