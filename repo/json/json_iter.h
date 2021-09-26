#pragma once
#include "json_common.h"

typedef struct JsonObjectIter JsonObjectIter, * pJsonObjectIter;

typedef void(*def_json_object_iter_remove)(pJsonObjectIter self);
typedef zzz_JSONType(*def_json_object_iter_getType)(pJsonObjectIter self);

typedef const char* (*def_json_object_iter_getKey)(pJsonObjectIter self);
typedef const char* (*def_json_object_iter_getUnEscapeKey)(pJsonObjectIter self);
typedef const char* (*def_json_object_iter_getString)(pJsonObjectIter self);
typedef const char* (*def_json_object_iter_getUnEscapeString)(pJsonObjectIter self);
typedef int (*def_json_object_iter_getInt)(pJsonObjectIter self);
typedef long (*def_json_object_iter_getLong)(pJsonObjectIter self);
typedef long long (*def_json_object_iter_getLL)(pJsonObjectIter self);
typedef BOOL(*def_json_object_iter_getBool)(pJsonObjectIter self);
typedef JsonArray* (*def_json_object_iter_getArray)(pJsonObjectIter self);
typedef Json* (*def_json_object_iter_getJson)(pJsonObjectIter self);
typedef double (*def_json_object_iter_getDouble)(pJsonObjectIter self);

typedef void (*def_json_object_iter_setKey)(pJsonObjectIter self, const char* new_key);
typedef void (*def_json_object_iter_setEscapeKey)(pJsonObjectIter self, const char* new_key);
typedef void (*def_json_object_iter_putString)(pJsonObjectIter self, const char* value);
typedef void (*def_json_object_iter_putEscapeString)(pJsonObjectIter self, const char* value);
typedef void (*def_json_object_iter_putInt)(pJsonObjectIter self, int value);
typedef void (*def_json_object_iter_putLong)(pJsonObjectIter self, long value);
typedef void (*def_json_object_iter_putLL)(pJsonObjectIter self, long long value);
typedef void (*def_json_object_iter_putBool)(pJsonObjectIter self, BOOL value);
typedef void (*def_json_object_iter_putArray)(pJsonObjectIter self, JsonArray* value);
typedef void (*def_json_object_iter_putJson)(pJsonObjectIter self, Json* value);
typedef void (*def_json_object_iter_putDouble)(pJsonObjectIter self, double value);

typedef JsonObjectIter* (*def_json_object_iter_next)(JsonObjectIter* self);

typedef struct JsonObjectIter {
	zzz_Value* own;
	JsonAllocater* allocater;
	zzz_BOOL last_error;

	def_json_object_iter_remove remove;
	def_json_object_iter_getType getType;

	def_json_object_iter_getKey getKey;
	def_json_object_iter_getUnEscapeKey getUnEscapeKey;
	def_json_object_iter_getString getString;
	def_json_object_iter_getUnEscapeString getUnEscapeString;
	def_json_object_iter_getInt getInt;
	def_json_object_iter_getLong getLong;
	def_json_object_iter_getLL getLL;
	def_json_object_iter_getBool getBool;
	def_json_object_iter_getArray getArray;
	def_json_object_iter_getJson getJson;
	def_json_object_iter_getDouble getDouble;

	def_json_object_iter_setKey setKey;
	def_json_object_iter_setEscapeKey setEscapeKey;
	def_json_object_iter_putString putString;
	def_json_object_iter_putEscapeString putEscapeString;
	def_json_object_iter_putInt putInt;
	def_json_object_iter_putLong putLong;
	def_json_object_iter_putLL putLL;
	def_json_object_iter_putBool putBool;
	def_json_object_iter_putArray putArray;
	def_json_object_iter_putJson putJson;
	def_json_object_iter_putDouble putDouble;

	def_json_object_iter_next next;
	JsonObjectIter** self_container;
}JsonObjectIter, * pJsonObjectIter;

#ifdef __cplusplus
extern "C"
{
#endif

	void CreateJsonObjectIter(JsonAllocater* allocater, zzz_Value* o, JsonObjectIter** pv);
	void DestoryJsonObjectIter(JsonObjectIter* v);

#ifdef __cplusplus
}
#endif
