#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>

using namespace v8;
// using namespace std;
class v8memory {
public:
	v8memory(Isolate* isolate);

	static v8memory* current(Isolate* isolate);
	void setup(Local<ObjectTemplate>, Local<String>);

	static void Getter(Local<String> property, const PropertyCallbackInfo<Value>& info);
	static void Setter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

	static void alloc(const FunctionCallbackInfo<v8::Value>& args);
	static void free(const FunctionCallbackInfo<v8::Value>& args);

	static void u32(const FunctionCallbackInfo<v8::Value>& args);
private:
	Isolate* _isolate;

};
