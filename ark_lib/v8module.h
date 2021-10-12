#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>

using namespace v8;

class v8module {
public:
	v8module(Isolate* isolate);

	static v8module* New(Isolate* isolate);
	void setup(Local<ObjectTemplate>, Local<String>);

	void Getter(Local<String> property, const PropertyCallbackInfo<Value>& info);
	void Setter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);
private:
	Local<Object> _module;
	Isolate* _isolate;
};
