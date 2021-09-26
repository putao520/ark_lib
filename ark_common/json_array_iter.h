#pragma once
#include "json_common.h"

typedef struct JsonArrayIter JsonArrayIter, * pJsonArrayIter;

typedef void(*def_json_array_iter_remove)(pJsonArrayIter self);
typedef zzz_JSONType(*def_json_array_iter_getType)(pJsonArrayIter self);

typedef unsigned int (*def_json_array_iter_getIndex)(pJsonArrayIter self);
typedef const char* (*def_json_array_iter_getString)(pJsonArrayIter self);
typedef const char* (*def_json_array_iter_getUnEscapeString)(pJsonArrayIter self);
typedef int (*def_json_array_iter_getInt)(pJsonArrayIter self);
typedef long (*def_json_array_iter_getLong)(pJsonArrayIter self);
typedef long long (*def_json_array_iter_getLL)(pJsonArrayIter self);
typedef BOOL(*def_json_array_iter_getBool)(pJsonArrayIter self);
typedef JsonArray* (*def_json_array_iter_getArray)(pJsonArrayIter self);
typedef Json* (*def_json_array_iter_getJson)(pJsonArrayIter self);
typedef double (*def_json_array_iter_getDouble)(pJsonArrayIter self);


typedef void (*def_json_array_iter_putString)(pJsonArrayIter self, const char* value);
typedef void (*def_json_array_iter_putEscapeString)(pJsonArrayIter self, const char* value);
typedef void (*def_json_array_iter_putInt)(pJsonArrayIter self, int value);
typedef void (*def_json_array_iter_putLong)(pJsonArrayIter self, long value);
typedef void (*def_json_array_iter_putLL)(pJsonArrayIter self, long long value);
typedef void (*def_json_array_iter_putBool)(pJsonArrayIter self, BOOL value);
typedef void (*def_json_array_iter_putArray)(pJsonArrayIter self, JsonArray* value);
typedef void (*def_json_array_iter_putJson)(pJsonArrayIter self, Json* value);
typedef void (*def_json_array_iter_putDouble)(pJsonArrayIter self, double value);

typedef JsonArrayIter* (*def_json_array_iter_next)(JsonArrayIter* self);

typedef struct JsonArrayIter {
	JsonAllocater* allocater;
	zzz_Value* own;
	zzz_BOOL last_error;
	unsigned int cursor;

	def_json_array_iter_remove remove;
	def_json_array_iter_getType getType;

	def_json_array_iter_getIndex getIndex;
	def_json_array_iter_getString getString;
	def_json_array_iter_getUnEscapeString getUnEscapeString;
	def_json_array_iter_getInt getInt;
	def_json_array_iter_getLong getLong;
	def_json_array_iter_getLL getLL;
	def_json_array_iter_getBool getBool;
	def_json_array_iter_getArray getArray;
	def_json_array_iter_getJson getJson;
	def_json_array_iter_getDouble getDouble;


	def_json_array_iter_putString putString;
	def_json_array_iter_putEscapeString putEscapeString;
	def_json_array_iter_putInt putInt;
	def_json_array_iter_putLong putLong;
	def_json_array_iter_putLL putLL;
	def_json_array_iter_putBool putBool;
	def_json_array_iter_putArray putArray;
	def_json_array_iter_putJson putJson;
	def_json_array_iter_putDouble putDouble;

	def_json_array_iter_next next;
	JsonArrayIter** self_container;
}JsonArrayIter, * pJsonArrayIter;

#ifdef __cplusplus
extern "C"
{
#endif
	void CreateJsonArrayIter(JsonAllocater* allocater, zzz_Value* o, JsonArrayIter** pv);
	void DestoryJsonArrayIter(JsonArrayIter* v);
#ifdef __cplusplus
}
#endif
