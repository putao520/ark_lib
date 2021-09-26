#include "json.h"
#include "json_array.h"

static inline zzz_Allocator* _GetAllocater(pJson self) {
	if (!self->allocater)
		InitJsonAllocater(&self->allocater);
	return self->allocater->allocater;
}

static inline zzz_Value* _GetCacheJsonObject(pJson self, const char* key, BOOL writer) {
	zzz_Value* r;
	if (strncmp(self->cache.cacheKey, key, MAX_CACHE_KEY_LENGTH) != 0) {
		r = zzz_ValueObjGet(self->glb_obj, key);
		if (!r && writer) {
			r = zzz_ValueNew(_GetAllocater(self));
			zzz_ValueSetKey(r, key);
			zzz_ValueObjAdd(self->glb_obj, r);
			r = zzz_ValueObjGet(self->glb_obj, key);
		}
		if (r)
			strcpy_s(self->cache.cacheKey, MAX_CACHE_KEY_LENGTH, key);

		self->cache.tmp_obj = r;
	}
	return self->cache.tmp_obj;
}


static Json* fn_fromString(pJson self, const char* json_string) {
	// New Value
	zzz_Value* src_v = zzz_ValueNew(_GetAllocater(self));
	unsigned int l = strlen(json_string) + 1;
	self->string_data = _malloc(l);
	strcpy_s((char *)self->string_data, l, json_string);
	BOOL ret = zzz_ValueParse(src_v, self->string_data);
	if (ret) {
		self->glb_obj = src_v;
	}
	return self;
}

static Json* fn_fromJson(pJson self, Json* json_object) {
	zzz_Value* src_v = zzz_ValueNew(_GetAllocater(self));
	BOOL ret = zzz_ValueCopyFrom(src_v, json_object->glb_obj);
	if (ret) {
		self->glb_obj = src_v;
	}
	return self;
}

static Json* fn_fromJsonArray(pJson self, JsonArray* json_array) {
	zzz_Value* src_v = zzz_ValueNew(_GetAllocater(self));
	BOOL ret = zzz_ValueCopyFrom(src_v, json_array->glb_obj);
	if (ret) {
		self->glb_obj = src_v;
	}
	return self;
}

static JsonArray* fn_toJsonArray(pJson self) {
	JsonArray* array = InitJsonArray();
	return array->fromJson(array, self);
}


static Json* fn_addJson(pJson self, Json* json_object) {
	JsonObjectIter* it = json_object->first(json_object);
	zzz_Value* src_v;
	BOOL ret;
	while (it) {
		src_v = zzz_ValueNew(_GetAllocater(self));
		ret = zzz_ValueCopyFrom(src_v, it->own);
		if (ret) {
			zzz_ValueObjAdd(self->glb_obj, src_v);
		}
		it = it->next(it);
	}
	return self;
}

static Json* fn_clear(pJson self) {
	UpdateJsonAllocater(self->allocater);
	self->glb_obj = zzz_ValueNew(_GetAllocater(self));
	self->error_status = zzz_ValueSetObj(self->glb_obj);

	return self;
}

static Json* fn_destory(pJson self) {
	DestoryJson(self);
	return NULL;
}

static int fn_getSize(pJson self) {
	return self->glb_obj ? zzz_ValueSize(self->glb_obj) : 0;
}

static BOOL fn_isNull(pJson self) {
	return self->glb_obj ? zzz_ValueIsNull(self->glb_obj) : FALSE;
}

static BOOL fn_isArray(pJson self) {
	if (!self->glb_obj)
		return False;
	zzz_JSONType* pt = zzz_ValueType(self->glb_obj);
	return pt ? *pt == zzz_JSONTypeArray : False;
}

static const char* fn_toSting(pJson self) {
	return self->glb_obj ? zzz_ValueStringify(self->glb_obj) : NULL;
}

static pJson fn_remove(pJson self, const char* key) {
	self->error_status = zzz_ValueObjDel(self->glb_obj, key);
	return self;
}


//----------------------------------------------------------------------
/*
	读系列接口
*/
static zzz_JSONType fn_getType(pJson self, const char* key) {
	zzz_Value* o = _GetCacheJsonObject(self, key, FALSE);
	if (!o)
		return zzz_JSONTypeNull;
	zzz_JSONType* pt = zzz_ValueType(o);
	return pt ? *pt : zzz_JSONTypeNull;
}
/*
static const char* fn_getKey(pJson self) {
	return self->glb_obj ? zzz_ValueGetKey(self->glb_obj) : NULL;
}
static const char* fn_getUnEscapeKey(pJson self) {
	return  self->glb_obj ? zzz_ValueGetUnEscapeKey(self->glb_obj) : NULL;
}
*/
static const char* fn_getString(pJson self, const char* key) {
	zzz_Value* o = _GetCacheJsonObject(self, key, FALSE);
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

static const char* fn_getUnEscapeString(pJson self, const char* key) {
	zzz_Value* o = _GetCacheJsonObject(self, key, FALSE);
	if (!o)
		return NULL;
	return zzz_ValueGetUnEscapeStr(o);
}

static int fn_getInt(pJson self, const char* key) {
	zzz_Value* o = _GetCacheJsonObject(self, key, FALSE);
	if (!o)
		return 0;
	const int* v = zzz_ValueGetInt(o);
	return v ? *v : 0;
}

static long fn_getLong(pJson self, const char* key) {
	zzz_Value* o = _GetCacheJsonObject(self, key, FALSE);
	if (!o)
		return 0;
	const long* v = zzz_ValueGetLong(o);
	return v ? *v : 0;
}

static long long fn_getLL(pJson self, const char* key) {
	zzz_Value* o = _GetCacheJsonObject(self, key, FALSE);
	if (!o)
		return 0;
	const long long* v = zzz_ValueGetLongLong(o);
	return v ? *v : 0;
}

static double fn_getDouble(pJson self, const char* key) {
	zzz_Value* o = _GetCacheJsonObject(self, key, FALSE);
	if (!o)
		return 0.0;
	const double* v = zzz_ValueGetDouble(o);
	return v ? *v : 0.0;
}

static BOOL fn_getBool(pJson self, const char* key) {
	zzz_Value* o = _GetCacheJsonObject(self, key, FALSE);
	if (!o)
		return False;
	BOOL* v = zzz_ValueGetBool(o);
	return v ? *v : False;
}

static JsonArray* fn_getArray(pJson self, const char* key) {
	zzz_Value* o = _GetCacheJsonObject(self, key, FALSE);
	if (!o)
		return NULL;
	JsonArray* json = InitJsonArrayWrap(self->allocater, o);
	// json->fromString(json, zzz_ValueStringify(o));
	return json;
}

static Json* fn_getJson(pJson self, const char* key) {
	zzz_Value* o = _GetCacheJsonObject(self, key, FALSE);
	if (!o)
		return NULL;
	Json* json = InitJsonWrap(self->allocater, o);
	// json->fromString(json, zzz_ValueStringify(o));
	return json;
}

/*
	写系列接口
*/
/*
static Json* fn_setKey(pJson self, zzz_Value* o, const char* new_key) {
	if (!o)
		return self;
	self->error_status = zzz_ValueSetKey(o, new_key);
	return self;
}
static Json* fn_setEscapeKey(pJson self, zzz_Value* o, const char* new_key) {
	if (!o)
		return self;
	self->error_status = zzz_ValueSetKeyEscape(o, new_key);
	return self;
}
*/

static Json* fn_putString(pJson self, const char* key, const char* value) {
	zzz_Value* o = _GetCacheJsonObject(self, key, TRUE);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSetStr(o, value);
	return self;
}

static Json* fn_putEscapeString(pJson self, const char* key, const char* value) {
	zzz_Value* o = _GetCacheJsonObject(self, key, TRUE);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSetStrEscape(o, value);
	return self;
}

static Json* fn_putInt(pJson self, const char* key, int value) {
	zzz_Value* o = _GetCacheJsonObject(self, key, TRUE);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSetInt(o, value);
	return self;
}

static Json* fn_putLong(pJson self, const char* key, long value) {
	zzz_Value* o = _GetCacheJsonObject(self, key, TRUE);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSetLong(o, value);
	return self;
}

static Json* fn_putLL(pJson self, const char* key, long long value) {
	zzz_Value* o = _GetCacheJsonObject(self, key, TRUE);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSetLongLong(o, value);
	return self;
}

static Json* fn_putDouble(pJson self, const char* key, double value) {
	zzz_Value* o = _GetCacheJsonObject(self, key, TRUE);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSetDouble(o, value);
	return self;
}

static Json* fn_putBool(pJson self, const char* key, BOOL value) {
	zzz_Value* o = _GetCacheJsonObject(self, key, TRUE);
	if (!o)
		return NULL;
	self->error_status = zzz_ValueSetBool(o, value);
	return self;
}

static Json* fn_putArray(pJson self, const char* key, JsonArray* value) {
	zzz_Value* o = _GetCacheJsonObject(self, key, TRUE);
	if (!o)
		return NULL;
	// 需要把来源值写入到本内存管理器里再复制
	zzz_Value* n = zzz_ValueCopyAuto(self->glb_obj, value->glb_obj);
	if (n)
		self->error_status = zzz_ValueSet(o, n);
	return self;
}

static Json* fn_putJson(pJson self, const char* key, Json* value) {
	zzz_Value* o = _GetCacheJsonObject(self, key, TRUE);
	if (!o)
		return NULL;
	// 需要把来源值写入到本内存管理器里再复制
	zzz_Value* n = zzz_ValueCopyAuto(self->glb_obj, value->glb_obj);
	if (n)
		self->error_status = zzz_ValueSet(o, n);
	return self;
}

static JsonObjectIter* fn_iter_first(pJson self) {
	if (self->iter)
		DestoryJsonObjectIter(self->iter);
	CreateJsonObjectIter(self->allocater, zzz_ValueBegin(self->glb_obj), &self->iter);
	return self->iter;
}

//======================================================================
static inline void _initActions(Json* self) {
	self->fromString = fn_fromString;
	self->fromJson = fn_fromJson;
	self->getSize = fn_getSize;
	self->isNull = fn_isNull;
	self->isArray = fn_isArray;
	self->toString = fn_toSting;
	self->addJson = fn_addJson;
	self->toJsonArray = fn_toJsonArray;

	self->getType = fn_getType;
	/*
	self->getKey = fn_getKey;
	self->getUnEscapeKey = fn_getUnEscapeKey;
	*/
	self->getString = fn_getString;
	self->getUnEscapeString = fn_getUnEscapeString;
	self->getInt = fn_getInt;
	self->getLong = fn_getLong;
	self->getLL = fn_getLL;
	self->getBool = fn_getBool;
	self->getArray = fn_getArray;
	self->getJson = fn_getJson;
	self->getDouble = fn_getDouble;

	/*
	self->setKey = fn_setKey;
	self->setEscapeKey = fn_setEscapeKey;
	*/
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
}

static Json* _InitJson() {
	Json* self = _malloc(sizeof(Json));
	memset(self, 0, sizeof(Json));
	_initActions(self);
	return self;
}

Json* InitJsonWrap(JsonAllocater* allocater, zzz_Value* v) {
	Json* j = _InitJson();
	j->allocater = allocater;
	j->glb_obj = v;
	return j;
}
Json* InitJson() {
	Json* self = _InitJson();
	self->fromString(self, "{}");
	return self;
}

void DestoryJson(Json* self) {
	if (self) {
		if (self->string_data)
			_free(self->string_data);

		if (self->iter)
			DestoryJsonObjectIter(self->iter);

		if (self->allocater)
			DestoryJsonAllocater(&self->allocater);

		_free(self);

	}
}
