#include "json_array_iter.h"
#include "json.h"
#include "json_array.h"

static void fn_remove(pJsonArrayIter self) {
	self->last_error = zzz_ValueMove(self->own);
}

static zzz_JSONType fn_getType(pJsonArrayIter self) {
	zzz_JSONType* pt = zzz_ValueType(self->own);
	return pt ? *pt : zzz_JSONTypeNull;
}

static unsigned int fn_getIndex(pJsonArrayIter self) {
	return self->cursor;
}


static const char* fn_getString(pJsonArrayIter self) {
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

static const char* fn_getUnEscapeString(pJsonArrayIter self) {
	return zzz_ValueGetUnEscapeStr(self->own);
}

static int fn_getInt(pJsonArrayIter self) {
	int* v = zzz_ValueGetInt(self->own);
	return v ? *v : 0;
}

static long fn_getLong(pJsonArrayIter self) {
	long* v = zzz_ValueGetLong(self->own);
	return v ? *v : 0;
}

static long long fn_getLL(pJsonArrayIter self) {
	long long* v = zzz_ValueGetLongLong(self->own);
	return v ? *v : 0;
}

static double fn_getDouble(pJsonArrayIter self) {
	double* v = zzz_ValueGetDouble(self->own);
	return v ? *v : 0.0;
}

static BOOL fn_getBool(pJsonArrayIter self) {
	BOOL* v = zzz_ValueGetBool(self->own);
	return v ? *v : False;
}

static JsonArray* fn_getArray(pJsonArrayIter self) {
	JsonArray* v = InitJsonArray();
	Json j = { 0 };
	j.glb_obj = self->own;
	return v->fromJson(v, &j);
}

static Json* fn_getJson(pJsonArrayIter self) {
	Json* v = InitJson();
	Json j = { 0 };
	j.glb_obj = self->own;
	return v->fromJson(v, &j);
}

static void fn_setKey(pJsonArrayIter self, const char* new_key) {
	self->last_error = zzz_ValueSetKey(self->own, new_key);
}
static void fn_setEscapeKey(pJsonArrayIter self, const char* new_key) {
	self->last_error = zzz_ValueSetKeyEscape(self->own, new_key);
}
static void fn_putString(pJsonArrayIter self, const char* value) {
	self->last_error = zzz_ValueSetStr(self->own, value);
}
static void fn_putEscapeString(pJsonArrayIter self, const char* value) {
	self->last_error = zzz_ValueSetStrEscape(self->own, value);
}
static void fn_putInt(pJsonArrayIter self, int value) {
	self->last_error = zzz_ValueSetInt(self->own, value);
}
static void fn_putLong(pJsonArrayIter self, long value) {
	self->last_error = zzz_ValueSetLong(self->own, value);
}
static void fn_putLL(pJsonArrayIter self, long long value) {
	self->last_error = zzz_ValueSetLongLong(self->own, value);
}
static void fn_putDouble(pJsonArrayIter self, double value) {
	self->last_error = zzz_ValueSetDouble(self->own, value);
}
static void fn_putBool(pJsonArrayIter self, BOOL value) {
	self->last_error = zzz_ValueSetBool(self->own, value);
}
static void fn_putArray(pJsonArrayIter self, JsonArray* value) {
	self->last_error = zzz_ValueSet(self->own, value->glb_obj);
}
static void fn_putJson(pJsonArrayIter self, Json* value) {
	self->last_error = zzz_ValueSet(self->own, value->glb_obj);
}

static JsonArrayIter* fn_iter_next(JsonArrayIter* self) {
	self->own = zzz_ValueNext(self->own);
	if (!self->own) {
		DestoryJsonArrayIter(self);
		self = NULL;
	}
	else
		self->cursor++;
	return self;
}

static inline void _initActions(JsonArrayIter* self) {
	self->remove = fn_remove;
	self->getType = fn_getType;

	self->getIndex = fn_getIndex;
	self->getString = fn_getString;
	self->getUnEscapeString = fn_getUnEscapeString;
	self->getInt = fn_getInt;
	self->getLong = fn_getLong;
	self->getLL = fn_getLL;
	self->getBool = fn_getBool;
	self->getArray = fn_getArray;
	self->getJson = fn_getJson;
	self->getDouble = fn_getDouble;

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

void CreateJsonArrayIter(JsonAllocater* allocater, zzz_Value* o, JsonArrayIter** pv) {
	if (!allocater || !o || !pv)
		return;

	JsonArrayIter* v = _malloc(sizeof(JsonArrayIter));
	_initActions(v);
	v->own = o;
	v->allocater = allocater;
	v->self_container = pv;
	v->cursor = 0;
	*pv = v;
}

void DestoryJsonArrayIter(JsonArrayIter* v) {
	if (v) {
		*v->self_container = (JsonArrayIter*)NULL;
		_free(v);
	}
}
