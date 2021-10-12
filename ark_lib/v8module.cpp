#include "v8module.h"

/**
* 
* context 嵌入数据:
*	100 -> module对象
*/
void v8module::Getter(Local<String> property,
	const PropertyCallbackInfo<Value>& info) {
	auto isolate = info.GetIsolate();
	info.GetReturnValue().Set(isolate->GetCurrentContext()->GetEmbedderData(100));
}

void v8module::Setter(Local<String> property, Local<Value> value,
	const PropertyCallbackInfo<void>& info) {
	auto isolate = info.GetIsolate();
	isolate->GetCurrentContext()->SetEmbedderData(100, value);
	x = value->Int32Value(
		info.GetIsolate()->GetCurrentContext()
	).ToChecked();
}

v8module::v8module(Isolate* isolate) : _isolate(isolate) {}

v8module* v8module::New(Isolate* isolate) {
	return new v8module(isolate);
}

void v8module::setup(Local<ObjectTemplate> parent, Local<String> name) {
	parent->SetAccessor(name, v8module::Getter, v8module::Setter);

	Local<ObjectTemplate> template_ = ObjectTemplate::New(_isolate);
	// exports set 方法
	template_->SetAccessorProperty(
		String::NewFromUtf8(_isolate, "exports", NewStringType::kInternalized).ToLocalChecked(),
		v8module::exports_getter,
		v8module::exports_setter
	);
}