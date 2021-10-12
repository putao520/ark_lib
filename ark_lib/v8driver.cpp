#include "v8driver.h"
#include "driver.h"

v8driver* global_v8t = nullptr;
driver* g_driver = nullptr;
v8driver::v8driver(Isolate* isolate) :
	_isolate(isolate) {
}

v8driver::~v8driver() {
}

void FindDriver(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	args.GetReturnValue().Set(Undefined(isolate));
	auto str = args[0].As<Value>()->ToString(isolate->GetCurrentContext());
	if (str.IsEmpty())
		return;
	Local<Value> driverName = str.ToLocalChecked();
	String::Utf8Value utf8(isolate, driverName);
	args.GetReturnValue().Set(BigInt::NewFromUnsigned(isolate, (uint64_t)g_driver->module(*utf8)));
}


void v8driver::setup(Local<ObjectTemplate> global) {
	// 构造全局对象
	global->Set(
		String::NewFromUtf8Literal(_isolate, "findDriver", NewStringType::kNormal),
		FunctionTemplate::New(_isolate, FindDriver)
	);
}

// 注意内存泄漏
v8driver* v8driver::current(Isolate* isolate) {
	if (!g_driver)
		g_driver = new driver();

	return global_v8t ? global_v8t : new v8driver(isolate);
}