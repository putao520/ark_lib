#pragma once
#include "json_common.h"
#include "json_iter.h"

EXTERN_C_START

	typedef pJson(*def_json_object_fromString)(pJson self, const char* json_string);
	typedef pJson(*def_json_object_fromJson)(pJson self, Json* json_object);
	typedef pJson(*def_json_object_fromJsonArray)(pJson self, JsonArray* json_array);
	typedef int (*def_json_object_getSize)(pJson self);
	typedef BOOL(*def_json_object_isNull)(pJson self);
	typedef BOOL(*def_json_object_isArray)(pJson self);
	typedef const char* (*def_json_object_toSting)(pJson self);

	typedef pJson(*def_json_object_destory)(pJson self);
	typedef pJson(*def_json_object_clear)(pJson self);
	typedef pJsonArray(*def_json_object_toJsonArray)(pJson self);
	typedef pJson(*def_json_object_addJson)(pJson self, Json* json_object);

	typedef pJson(*def_json_object_remove)(pJson self, const char* key);
	typedef zzz_JSONType(*def_json_object_getType)(pJson self, const char* key);
	typedef const char* (*def_json_object_getString)(pJson self, const char* key);
	typedef const char* (*def_json_object_getUnEscapeString)(pJson self, const char* key);
	typedef int (*def_json_object_getInt)(pJson self, const char* key);
	typedef long (*def_json_object_getLong)(pJson self, const char* key);
	typedef long long (*def_json_object_getLL)(pJson self, const char* key);
	typedef BOOL(*def_json_object_getBool)(pJson self, const char* key);
	typedef JsonArray* (*def_json_object_getArray)(pJson self, const char* key);
	typedef Json* (*def_json_object_getJson)(pJson self, const char* key);
	typedef double (*def_json_object_getDouble)(pJson self, const char* key);

	/*
	typedef Json* (*def_json_object_setKey)(pJson self, zzz_Value* o, const char* new_key);
	typedef Json* (*def_json_object_setEscapeKey)(pJson self, zzz_Value* o, const char* new_key);
	*/
	typedef Json* (*def_json_object_putString)(pJson self, const char* key, const char* value);
	typedef Json* (*def_json_object_putEscapeString)(pJson self, const char* key, const char* value);
	typedef Json* (*def_json_object_putInt)(pJson self, const char* key, int value);
	typedef Json* (*def_json_object_putLong)(pJson self, const char* key, long value);
	typedef Json* (*def_json_object_putLL)(pJson self, const char* key, long long value);
	typedef Json* (*def_json_object_putBool)(pJson self, const char* key, BOOL value);
	typedef Json* (*def_json_object_putArray)(pJson self, const char* key, JsonArray* value);
	typedef Json* (*def_json_object_putJson)(pJson self, const char* key, Json* value);
	typedef Json* (*def_json_object_putDouble)(pJson self, const char* key, double value);


	typedef JsonObjectIter* (*def_json_object_iter_first)(pJson self);

#define MAX_CACHE_KEY_LENGTH 256

	typedef struct CacheKeyJson {
		char cacheKey[MAX_CACHE_KEY_LENGTH];
		zzz_Value* tmp_obj;
	}CacheKeyJson;


	typedef struct Json {
		JsonAllocater* allocater;
		zzz_Value* glb_obj;
		CacheKeyJson cache;
		zzz_BOOL error_status;
		JsonObjectIter* iter;
		const char* string_data;

		def_json_object_fromString fromString;
		def_json_object_fromJson fromJson;
		def_json_object_fromJsonArray fromJsonArray;
		def_json_object_getSize getSize;
		def_json_object_isNull isNull;
		def_json_object_isArray isArray;
		def_json_object_toSting toString;
		def_json_object_addJson addJson;
		def_json_object_toJsonArray toJsonArray;
		def_json_object_destory destory;

		def_json_object_getType getType;
		/*
		def_json_object_getKey getKey;
		def_json_object_getUnEscapeKey getUnEscapeKey;
		*/
		def_json_object_getString getString;
		def_json_object_getUnEscapeString getUnEscapeString;
		def_json_object_getInt getInt;
		def_json_object_getLong getLong;
		def_json_object_getLL getLL;
		def_json_object_getBool getBool;
		def_json_object_getArray getArray;
		def_json_object_getJson getJson;
		def_json_object_getDouble getDouble;

		/*
		def_json_object_setKey setKey;
		def_json_object_setEscapeKey setEscapeKey;
		*/
		def_json_object_putString putString;
		def_json_object_putEscapeString putEscapeString;
		def_json_object_putInt putInt;
		def_json_object_putLong putLong;
		def_json_object_putLL putLL;
		def_json_object_putBool putBool;
		def_json_object_putArray putArray;
		def_json_object_putJson putJson;
		def_json_object_putDouble putDouble;

		def_json_object_remove remove;
		def_json_object_clear clear;

		def_json_object_iter_first first;
		def_json_object_iter_next next;
	}Json, * pJson;

	Json* InitJsonWrap(JsonAllocater* allocater, zzz_Value* v);
	Json* InitJson();
	void DestoryJson(Json* self);

EXTERN_C_END
