#include "v8procedure.h"
#include "ActorService.h"
#include "text_encode.h"
#include <deque>
#ifdef _DEBUG
#include "debug_until.h"
#endif

thread_local v8procedure* global_v8t = nullptr;
v8procedure::v8procedure(Isolate* isolate) :
	_isolate(isolate) {
}

v8procedure::~v8procedure() {
}

void parameter(actor_client* actor, Isolate* isolate, Local<Value> v, deque<void*>* unicode_ptr_array) {
	auto context = isolate->GetCurrentContext();
	// 数字
	if (v->IsInt32()) {
#ifdef _DEBUG
		printf("int32:%x\n", v->Int32Value(context).ToChecked());
#endif
		actor->push(v->Int32Value(context).ToChecked());
	}
	// 64位数字
	else if (v->IsBigInt()) {
#ifdef _DEBUG
		printf("BigInt:%#llx\n",v->ToBigInt(context).ToLocalChecked()->Uint64Value());
#endif
		actor->push(v->ToBigInt(context).ToLocalChecked()->Uint64Value());
	}
	// utf8字符串
	else if (v->IsString()) {
		String::Utf8Value utf8(isolate, v);
		wchar_t* buffer = (wchar_t*)TextEncode::utf16le(*utf8);
		if (buffer) {
#ifdef _DEBUG
			printf("unicode:%ws\n", buffer);
#endif
			actor->push(buffer);
			if(unicode_ptr_array)
				unicode_ptr_array->push_back(buffer);
		}
	}
	// uint8array数组
	else if (v->IsArrayBuffer()) {
		auto buffer_ = v.As<ArrayBuffer>();
		void* buffer = buffer_->GetBackingStore()->Data();
#ifdef _DEBUG
		printf("ArrayBuffer: %p\n", buffer);
#endif
		actor->push(buffer);
	}
	// tuple 数组
	else if (v->IsArray()) {
		auto arr = v.As<v8::Array>();
		if (arr->Length() > 0) {
			for (uint32_t i = 0; i < arr->Length(); i++) {
				parameter(actor, isolate, arr->Get(context, i).ToLocalChecked(), unicode_ptr_array);
			}
		}
	}
}

void procedure(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	if (args.Length() < 2)
		return;

	auto context = isolate->GetCurrentContext();
	if (context.IsEmpty())
		return;

	// 获得调用地址
	auto arg_0 = args[0].As<Value>()->ToBigInt(context);
	if (arg_0.IsEmpty())
		return;
	uint64_t address = arg_0.ToLocalChecked()->Uint64Value();

#ifdef _DEBUG
	printf("procedure: %#llx\n", address);
	__pauseDebug("procedure: %#llx\n", address);
#endif

	// 执行中断等级
	auto arg_1 = args[1].As<Value>()->Uint32Value(context);
	if (arg_1.IsNothing())
		return;
	uint32_t irql = arg_1.ToChecked();

	deque<void*> unicode_ptr_array;
	auto actor = ActorService::current();
	// 参数处理
	for (int i = 2; i < args.Length(); i++) {
		auto v = args[i];
		parameter(actor, isolate, args[i], &unicode_ptr_array);
	}
	// 调用函数
	args.GetReturnValue().Set(BigInt::New(isolate, actor->run((void*)address, (unsigned char)irql)));

	// 释放所有 wchart_ 内存
	for (void* pb : unicode_ptr_array) {
		if(pb)
			TextEncode::free(pb);
	}
}

void v8procedure::setup(Local<ObjectTemplate> global) {
	// 构造全局对象
	global->Set(
		String::NewFromUtf8Literal(_isolate, "procedure", NewStringType::kNormal),
		FunctionTemplate::New(_isolate, procedure)
	);
}

// 注意内存泄漏
v8procedure* v8procedure::current(Isolate* isolate) {
	return global_v8t ? global_v8t : new v8procedure(isolate);
}