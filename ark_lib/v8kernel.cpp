#include "v8kernel.h"
#include "ActorService.h"

Global<ObjectTemplate> kernel_object;

v8kernel* global_kernel = nullptr;
void Getter(Local<String> property, const PropertyCallbackInfo<Value>& info) {
	// info.GetReturnValue().Set(x);
	Local<ObjectTemplate> templ = Local<ObjectTemplate>::New(info.GetIsolate(), kernel_object);

	Local<Object> result =
		templ->NewInstance(info.GetIsolate()->GetCurrentContext()).ToLocalChecked();
}

void Setter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
	/*
	x = value->ToObject(
		info.GetIsolate()->GetCurrentContext()
	);
	*/

}

void ReadMemory(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if ( !ctx.IsEmpty() ) {
		// 获得参数
		uint64_t buffer = static_cast<uint64_t>(args[0].As<Value>()->IntegerValue(ctx).ToChecked());
		uint64_t address = static_cast<uint64_t>(args[1].As<Value>()->IntegerValue(ctx).ToChecked());
		uint32_t size = args[2].As<Value>()->Uint32Value(ctx).ToChecked();
		// 调用实际函数
		auto r = ActorService::current()->push(buffer)
			->push(address)
			->push(size)
			->run(ActorService::services()->read);
		auto result = Uint32::New(isolate, (uint32_t)r);
		args.GetReturnValue().Set(result);
	}
}

v8kernel::v8kernel(Isolate* isolate) : _isolate(isolate) {}

Local<ObjectTemplate> v8kernel::MakeTemplate() {
	EscapableHandleScope handle_scope(_isolate);

	Local<ObjectTemplate> result = ObjectTemplate::New(_isolate);

	result->Set(
		String::NewFromUtf8Literal(_isolate, "read", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, ReadMemory)
	);

	return handle_scope.Escape(result);
}

v8kernel* v8kernel::current(Isolate* isolate) {
	return global_kernel ? global_kernel : new v8kernel(isolate);
}

void v8kernel::setup(Local<ObjectTemplate> parent, Local<String> name) {
	if (kernel_object.IsEmpty()) {
		Local<ObjectTemplate> raw_template = MakeTemplate();
		kernel_object.Reset(_isolate, raw_template);
	}

	parent->SetAccessor(name, Getter, Setter);
}