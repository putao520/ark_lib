#include "json_iter.h"
#include "json.h"
#include "json_array.h"

static void fn_remove(pJsonObjectIter self) {
	self->last_error = zzz_ValueMove(self->own);
}

static zzz_JSONType fn_getType(pJsonObjectIter self) {
	zzz_JSONType* pt = zzz_ValueType(self->own);
	return pt ? *pt : zzz_JSONTypeNull;
}

static const char* fn_getKey(pJsonObjectIter self) {
	return zzz_ValueGetKey(self->own);
}

static const char* fn_getUnEscapeKey(pJsonObjectIter self) {
	return zzz_ValueGetUnEscapeKey(self->own);
}

static const char* fn_getString(pJsonObjectIter self) {
	const char* r;
	zzz_JSONType* pt = zzz_ValueType(self->own);
	if (!pt)
		return zzz_StrNull;

	switch (*pt) {
	case zzz_JSONTYPEARRAY:
	case zzz_JSONTYPEOBJECT:
		r = zzz_ValueStringify(self->own);
		break;
	case zzz_JSONTYPESTRING:
		r = zzz_ValueGetStr(self->own);
		break;
	case zzz_JSONTYPENUMBER:
		r = zzz_ValueGetNumStr(self->own);
		break;
	case zzz_JSONTYPEBOOL:
		r = zzz_ValueGetBool(self->own);
		break;
	default:
		r = zzz_StrNull;
		break;
	}
	return r;
}

static const char* fn_getUnEscapeString(pJsonObjectIter self) {
	return zzz_ValueGetUnEscapeStr(self->own);
}

static int fn_getInt(pJsonObjectIter self) {
	int* v = zzz_ValueGetInt(self->own);
	return v ? *v : 0;
}

static long fn_getLong(pJsonObjectIter self) {
	long* v = zzz_ValueGetLong(self->own);
	return v ? *v : 0;
}

static long long fn_getLL(pJsonObjectIter self) {
	long long* v = zzz_ValueGetLongLong(self->own);
	return v ? *v : 0;
}

static double fn_getDouble(pJsonObjectIter self) {
	double* v = zzz_ValueGetDouble(self->own);
	return v ? *v : 0.0;
}

static BOOL fn_getBool(pJsonObjectIter self) {
	BOOL* v = zzz_ValueGetBool(self->own);
	return v ? *v : False;
}

static JsonArray* fn_getArray(pJsonObjectIter self) {
	JsonArray* v = InitJsonArrayWrap(self->allocater, self->own);
	/*
	Json j = { 0 };
	j.glb_obj = self->own;
	return v->fromJson(v, &j);
	*/

	return v;
}

static Json* fn_getJson(pJsonObjectIter self) {
	Json* v = InitJsonWrap(self->allocater, self->own);
	/*
	Json j = { 0 };
	j.glb_obj = self->own;
	return v->fromJson(v, &j);
	*/
	return v;
}

static void fn_setKey(pJsonObjectIter self, const char* new_key) {
	self->last_error = zzz_ValueSetKey(self->own, new_key);
}
static void fn_setEscapeKey(pJsonObjectIter self, const char* new_key) {
	self->last_error = zzz_ValueSetKeyEscape(self->own, new_key);
}
static void fn_putString(pJsonObjectIter self, const char* value) {
	self->last_error = zzz_ValueSetStr(self->own, value);
}
static void fn_putEscapeString(pJsonObjectIter self, const char* value) {
	self->last_error = zzz_ValueSetStrEscape(self->own, value);
}
static void fn_putInt(pJsonObjectIter self, int value) {
	self->last_error = zzz_ValueSetInt(self->own, value);
}
static void fn_putLong(pJsonObjectIter self, long value) {
	self->last_error = zzz_ValueSetLong(self->own, value);
}
static void fn_putLL(pJsonObjectIter self, long long value) {
	self->last_error = zzz_ValueSetLongLong(self->own, value);
}
static void fn_putDouble(pJsonObjectIter self, double value) {
	self->last_error = zzz_ValueSetDouble(self->own, value);
}
static void fn_putBool(pJsonObjectIter self, BOOL value) {
	self->last_error = zzz_ValueSetBool(self->own, value);
}
static void fn_putArray(pJsonObjectIter self, JsonArray* value) {
	self->last_error = zzz_ValueSet(self->own, value->glb_obj);
}
static void fn_putJson(pJsonObjectIter self, Json* value) {
	self->last_error = zzz_ValueSet(self->own, value->glb_obj);
}

static JsonObjectIter* fn_iter_next(JsonObjectIter* self) {
	self->own = zzz_ValueNext(self->own);
	if (!self->own) {
		DestoryJsonObjectIter(self);
		self = NULL;
	}
	return self;
}

static inline void _initActions(JsonObjectIter* self) {
	self->remove = fn_remove;
	self->getType = fn_getType;

	self->getKey = fn_getKey;
	self->getUnEscapeKey = fn_getUnEscapeKey;
	self->getString = fn_getString;
	self->getUnEscapeString = fn_getUnEscapeString;
	self->getInt = fn_getInt;
	self->getLong = fn_getLong;
	self->getLL = fn_getLL;
	self->getBool = fn_getBool;
	self->getArray = fn_getArray;
	self->getJson = fn_getJson;
	self->getDouble = fn_getDouble;

	self->setKey = fn_setKey;
	self->setEscapeKey = fn_setEscapeKey;
	self->putString = fn_putString;
	self->putEscapeString = fn_putEscapeString;
	self->putInt = fn_putInt;
	self->putLong = fn_putLong;
	self->putLL = fn_putLL;
	self->putBool = fn_putBool;
	self->putArray = fn_putArray;
	self->putJson = fn_putJson;
	self->putDouble = fn_putDouble;

	self->next = fn_iter_next;
}

void CreateJsonObjectIter(JsonAllocater* allocater, zzz_Value* o, JsonObjectIter** pv) {
	if (!allocater || !o || !pv)
		return;
	JsonObjectIter* v = _malloc(sizeof(JsonObjectIter));
	_initActions(v);
	v->own = o;
	v->allocater = allocater;
	v->self_container = pv;
	*pv = v;
}

void DestoryJsonObjectIter(JsonObjectIter* v) {
	if (v) {
		*v->self_container = (JsonObjectIter*)NULL;
		_free(v);
	}
}
