#include "json.h"
#include "json_array.h"

static inline zzz_Allocator* _GetAllocater(pJsonArray self) {
	if (!self->allocater)
		InitJsonAllocater(&self->allocater);
	return self->allocater->allocater;
}

static inline int _GetNewJsonArrayObject(pJsonArray self, def_set_common cb, void* v) {
	zzz_Value* r = zzz_ValueNew(_GetAllocater(self));
	zzz_SIZE maxi = zzz_ValueSize(self->glb_obj);
	return cb(r, v) && zzz_ValueArrayAdd(self->glb_obj, r) ? (int)maxi : -1;
}

static inline int _GetNewJsonArrayObject8B(pJsonArray self, def_set_common8B cb, double v) {
	zzz_Value* r = zzz_ValueNew(_GetAllocater(self));
	zzz_SIZE maxi = zzz_ValueSize(self->glb_obj);
	return cb(r, v) && zzz_ValueArrayAdd(self->glb_obj, r) ? (int)maxi : -1;
}

static inline zzz_Value* _GetCacheJsonArrayObject(pJsonArray self, unsigned int idx) {
	return zzz_ValueArrayGet(self->glb_obj, idx);
}


static JsonArray* fn_fromString(pJsonArray self, const char* json_string) {
	// New Value
	zzz_Value* src_v = zzz_ValueNew(_GetAllocater(self));
	unsigned int l = strlen(json_string) + 1;
	self->string_data = _malloc(l);
	strcpy_s(self->string_data, l, json_string);
	BOOL ret = zzz_ValueParse(src_v, self->string_data);
	if (ret) {
		self->glb_obj = src_v;
	}
	return self;
}

static JsonArray* fn_fromJson(pJsonArray self, Json* json_object) {
	zzz_Value* src_v = zzz_ValueNew(_GetAllocater(self));
	BOOL ret = zzz_ValueCopyFrom(src_v, json_object->glb_obj);
	if (ret) {
		self->glb_obj = src_v;
	}
	return self;
}

static JsonArray* fn_fromJsonArray(pJsonArray self, JsonArray* json_array_object) {
	zzz_Value* src_v = zzz_ValueNew(_GetAllocater(self));
	BOOL ret = zzz_ValueCopyFrom(src_v, json_array_object->glb_obj);
	if (ret) {
		self->glb_obj = src_v;
	}
	return self;
}

static JsonArray* fn_addJsonArray(pJsonArray self, JsonArray* json_object) {
	JsonArrayIter* it = json_object->first(json_object);
	zzz_Value* src_v;
	BOOL ret;
	while (it) {
		src_v = zzz_ValueNew(_GetAllocater(self));
		ret = zzz_ValueCopyFrom(src_v, it->own);
		if (ret) {
			zzz_ValueArrayAdd(self->glb_obj, src_v);
		}
		it = it->next(it);
	}
	return self;
}

static JsonArray* fn_clear(pJsonArray self) {
	UpdateJsonAllocater(self->allocater);
	self->glb_obj = zzz_ValueNew(_GetAllocater(self));
	self->error_status = zzz_ValueSetArray(self->glb_obj);

	return self;
}

static int fn_getSize(pJsonArray self) {
	return self->glb_obj ? zzz_ValueSize(self->glb_obj) : 0;
}

static BOOL fn_isNull(pJsonArray self) {
	return self->glb_obj ? zzz_ValueIsNull(self->glb_obj) : 0;
}

static BOOL fn_isObject(pJsonArray self) {
	if (!self->glb_obj)
		return False;
	zzz_JSONType* pt = zzz_ValueType(self->glb_obj);
	return pt ? *pt == zzz_JSONTypeObject : False;
}

static const char* fn_toSting(pJsonArray self) {
	return self->glb_obj ? zzz_ValueStringify(self->glb_obj) : 0;
}

static pJson fn_toJson(pJsonArray self) {
	Json* object = InitJson();
	return object->fromJsonArray(object, self);
}

static pJsonArray fn_remove(pJsonArray self, unsigned int idx) {
	self->error_status = zzz_ValueArrayDel(self->glb_obj, idx);
	return self;
}

static pJsonArray fn_destory(pJsonArray self) {
	DestoryJsonArray(self);
	return NULL;
}

//----------------------------------------------------------------------
/*
	读系列接口
*/

static zzz_JSONType fn_getType(pJsonArray self, unsigned int idx) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return zzz_JSONTypeNull;
	zzz_JSONType* pt = zzz_ValueType(o);
	return pt ? *pt : zzz_JSONTypeNull;
}

static const char* fn_getString(pJsonArray self, unsigned int idx) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return NULL;

	const char* r;
	zzz_JSONType* pt = zzz_ValueType(o);
	if (!pt)
		return zzz_StrNull;

	switch (*pt) {
	case zzz_JSONTYPEARRAY:
	case zzz_JSONTYPEOBJECT:
		r = zzz_ValueStringify(o);
		break;
	case zzz_JSONTYPESTRING:
		r = zzz_ValueGetStr(o);
		break;
	case zzz_JSONTYPENUMBER:
		r = zzz_ValueGetNumStr(o);
		break;
	case zzz_JSONTYPEBOOL:
		r = zzz_ValueGetBool(o);
		break;
	default:
		r = zzz_StrNull;
		break;
	}
	return r;
}

static const char* fn_getUnEscapeString(pJsonArray self, unsigned int idx) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return NULL;
	return zzz_ValueGetUnEscapeStr(o);
}

static int fn_getInt(pJsonArray self, unsigned int idx) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return 0;
	const int* v = zzz_ValueGetInt(o);
	return v ? *v : 0;
}

static long fn_getLong(pJsonArray self, unsigned int idx) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return 0;
	const long* v = zzz_ValueGetLong(o);
	return v ? *v : 0;
}

static long long fn_getLL(pJsonArray self, unsigned int idx) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return 0;
	const long long* v = zzz_ValueGetLongLong(o);
	return v ? *v : 0;
}

static double fn_getDouble(pJsonArray self, unsigned int idx) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return 0.0;
	const double* v = zzz_ValueGetDouble(o);
	return v ? *v : 0.0;
}

static BOOL fn_getBool(pJsonArray self, unsigned int idx) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return False;
	BOOL* v = zzz_ValueGetBool(o);
	return v ? *v : False;
}

static JsonArray* fn_getArray(pJsonArray self, unsigned int idx) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return NULL;
	JsonArray* json = InitJsonArrayWrap(self->allocater, o);
	// json->fromString(json, zzz_ValueStringify(o));
	return json;
}

static Json* fn_getJson(pJsonArray self, unsigned int idx) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return NULL;
	Json* json = InitJsonWrap(self->allocater, o);
	// json->fromString(json, zzz_ValueStringify(o));
	/*
	Json j = { 0 };
	j.glb_obj = o;
	json->fromJson(json, &j);
	*/
	return json;
}

/*
	写系列接口
*/
static JsonArray* fn_setKey(pJsonArray self, zzz_Value* o, const char* new_key) {
	if (!o)
		return self;
	self->error_status = zzz_ValueSetKey(o, new_key);
	return self;
}

static JsonArray* fn_setEscapeKey(pJsonArray self, zzz_Value* o, const char* new_key) {
	if (!o)
		return self;
	self->error_status = zzz_ValueSetKeyEscape(o, new_key);
	return self;
}


static JsonArray* fn_replaceString(pJsonArray self, unsigned int idx, const char* value) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSetStr(o, value);
	return self;
}

static JsonArray* fn_replaceEscapeString(pJsonArray self, unsigned int idx, const char* value) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSetStrEscape(o, value);
	return self;
}

static JsonArray* fn_replaceInt(pJsonArray self, unsigned int idx, int value) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSetInt(o, value);
	return self;
}

static JsonArray* fn_replaceLong(pJsonArray self, unsigned int idx, long value) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSetLong(o, value);
	return self;
}

static JsonArray* fn_replaceLL(pJsonArray self, unsigned int idx, long long value) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSetLongLong(o, value);
	return self;
}

static JsonArray* fn_replaceDouble(pJsonArray self, unsigned int idx, double value) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSetDouble(o, value);
	return self;
}

static JsonArray* fn_replaceBool(pJsonArray self, unsigned int idx, BOOL value) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSetBool(o, value);
	return self;
}

static JsonArray* fn_replaceArray(pJsonArray self, unsigned int idx, JsonArray* value) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSet(o, value->glb_obj);
	return self;
}

static JsonArray* fn_replaceJson(pJsonArray self, unsigned int idx, Json* value) {
	zzz_Value* o = _GetCacheJsonArrayObject(self, idx);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSet(o, value->glb_obj);
	return self;
}

//-----------------------------------
static zzz_BOOL fn_CmPutString(zzz_Value* o, void* v) {
	return zzz_ValueSetStr(o, (const char*)v);
}
static int fn_putString(pJsonArray self, const char* value) {
	return _GetNewJsonArrayObject(self, fn_CmPutString, value);
}
static zzz_BOOL fn_CmPutEscapeString(zzz_Value* o, void* v) {
	return zzz_ValueSetStrEscape(o, (const char*)v);
}
static int fn_putEscapeString(pJsonArray self, const char* value) {
	return _GetNewJsonArrayObject(self, fn_CmPutEscapeString, value);
}
static zzz_BOOL fn_CmPutInt(zzz_Value* o, void* v) {
	return zzz_ValueSetInt(o, (const int)v);
}
static int fn_putInt(pJsonArray self, int value) {
	return _GetNewJsonArrayObject(self, fn_CmPutInt, (void*)value);
}
static zzz_BOOL fn_CmPutLong(zzz_Value* o, void* v) {
	return zzz_ValueSetLong(o, (const long)v);
}
static int fn_putLong(pJsonArray self, long value) {
	return _GetNewJsonArrayObject(self, fn_CmPutLong, (void*)value);
}
static zzz_BOOL fn_CmPutLL(zzz_Value* o, void* v) {
	return zzz_ValueSetLongLong(o, (const long long)v);
}
static int fn_putLL(pJsonArray self, long long value) {
	return _GetNewJsonArrayObject(self, fn_CmPutLL, (void*)value);
}
static zzz_BOOL fn_CmPutDouble(zzz_Value* o, double v) {
	return zzz_ValueSetDouble(o, (const double)v);
}
static int fn_putDouble(pJsonArray self, double value) {
	return _GetNewJsonArrayObject8B(self, fn_CmPutDouble, value);
}
static zzz_BOOL fn_CmPutBool(zzz_Value* o, void* v) {
	return zzz_ValueSetBool(o, (zzz_BOOL)v);
}
static int fn_putBool(pJsonArray self, BOOL value) {
	return _GetNewJsonArrayObject(self, fn_CmPutBool, (void*)value);
}
static zzz_BOOL fn_CmPutJson(zzz_Value* o, void* v) {
	// 需要把来源值写入到本内存管理器里再复制
	zzz_Value* n = zzz_ValueCopyAuto(o, (const zzz_Value*)v);
	if (n)
		return zzz_ValueSet(o, n);
	return False;
}
static int fn_putArray(pJsonArray self, JsonArray* value) {
	return _GetNewJsonArrayObject(self, fn_CmPutJson, (void*)value->glb_obj);
}
static int fn_putJson(pJsonArray self, Json* value) {
	return _GetNewJsonArrayObject(self, fn_CmPutJson, (void*)value->glb_obj);
}

static JsonArrayIter* fn_iter_first(pJsonArray self) {
	if (self->iter)
		DestoryJsonArrayIter(self->iter);
	CreateJsonArrayIter(self->allocater, zzz_ValueBegin(self->glb_obj), &self->iter);
	return self->iter;
}

//======================================================================
static inline void _initActions(JsonArray* self) {
	self->fromString = fn_fromString;
	self->fromJson = fn_fromJson;
	self->fromJsonArray = fn_fromJsonArray;
	self->getSize = fn_getSize;
	self->isNull = fn_isNull;
	self->isObject = fn_isObject;
	self->toString = fn_toSting;
	self->addJsonArray = fn_addJsonArray;
	self->toJson = fn_toJson;

	self->getType = fn_getType;
	self->getString = fn_getString;
	self->getUnEscapeString = fn_getUnEscapeString;
	self->getInt = fn_getInt;
	self->getLong = fn_getLong;
	self->getLL = fn_getLL;
	self->getBool = fn_getBool;
	self->getArray = fn_getArray;
	self->getJson = fn_getJson;
	self->getDouble = fn_getDouble;

	self->replaceString = fn_replaceString;
	self->replaceEscapeString = fn_replaceEscapeString;
	self->replaceInt = fn_replaceInt;
	self->replaceLong = fn_replaceLong;
	self->replaceLL = fn_replaceLL;
	self->replaceBool = fn_replaceBool;
	self->replaceArray = fn_replaceArray;
	self->replaceJson = fn_replaceJson;
	self->replaceDouble = fn_replaceDouble;

	self->putString = fn_putString;
	self->putEscapeString = fn_putEscapeString;
	self->putInt = fn_putInt;
	self->putLong = fn_putLong;
	self->putLL = fn_putLL;
	self->putBool = fn_putBool;
	self->putArray = fn_putArray;
	self->putJson = fn_putJson;
	self->putDouble = fn_putDouble;

	self->remove = fn_remove;
	self->clear = fn_clear;
	self->first = fn_iter_first;

	self->destory = fn_destory;

	self->fromString(self, "[]");
}

JsonArray* InitJsonArrayWrap(JsonAllocater* allocater, zzz_Value* v) {
	JsonArray* j = InitJsonArray();
	j->allocater = allocater;
	j->glb_obj = v;
	return j;
}

JsonArray* InitJsonArray() {
	JsonArray* self = _malloc(sizeof(JsonArray));
	memset(self, 0, sizeof(JsonArray));
	_initActions(self);
	self->iter = NULL;
	self->cache.tmp_idx = -1;
	self->string_data = NULL;
	return self;
}

void DestoryJsonArray(JsonArray* self) {
	if (self) {
		if (self->string_data)
			_free(self->string_data);

		if (self->iter)
			DestoryJsonArrayIter(self->iter);

		if (self->allocater)
			DestoryJsonAllocater(&self->allocater);

		_free(self);
	}
}
