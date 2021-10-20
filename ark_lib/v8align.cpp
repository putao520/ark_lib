#include "v8align.h"
// #include "text_encode.h"

v8align* global_v8t = nullptr;
v8align::v8align(Isolate* isolate) :
	_isolate(isolate) {
}

v8align::~v8align() {
}

void Align(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	if (args.Length() == 2) {
		auto ctx = isolate->GetCurrentContext();
		auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
		if (!arg_0.IsEmpty()) {
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();
			auto arg_1 = args[1].As<Value>()->Uint32Value(ctx);
			if (!arg_1.IsNothing()) {
				uint32_t v = arg_1.ToChecked();
				Local<Value> result = BigInt::NewFromUnsigned(isolate, (address + (uint64_t)v - 1ull) & (~((uint64_t)v - 1ull)));
				args.GetReturnValue().Set(result);
				return;
			}
		}
	}
	args.GetReturnValue().Set(Undefined(isolate));
}


void v8align::setup(Local<ObjectTemplate> global) {
	// 构造全局对象
	global->Set(
		String::NewFromUtf8Literal(_isolate, "align", NewStringType::kNormal),
		FunctionTemplate::New(_isolate, Align)
	);
}

// 注意内存泄漏
v8align* v8align::current(Isolate* isolate) {
	return global_v8t ? global_v8t : new v8align(isolate);
}