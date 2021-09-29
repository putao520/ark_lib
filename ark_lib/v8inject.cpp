#include "v8inject.h"

int x = 1000;

v8inject* global_inject = nullptr;
void Getter(Local<String> property,
	const PropertyCallbackInfo<Value>& info) {
	info.GetReturnValue().Set(x);
}

void Setter(Local<String> property, Local<Value> value,
	const PropertyCallbackInfo<void>& info) {
	x = value->Int32Value(
		info.GetIsolate()->GetCurrentContext()
	).ToChecked();
}

v8inject::v8inject(Isolate* isolate) : _isolate(isolate) {}

v8inject* v8inject::current(Isolate* isolate) {
	return global_inject ? global_inject : new v8inject(isolate);
}

void v8inject::setup(Local<ObjectTemplate> parent, Local<String> name) {
	parent->SetAccessor(name, Getter, Setter);
}