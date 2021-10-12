#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>

using namespace v8;
using namespace std;

class v8buffer {
public:
	v8buffer(Isolate* isolate);
	~v8buffer();
	void setup(Local<ObjectTemplate>);

	static v8buffer* current(Isolate* isolate);

	static void Getter(Local<String> property, const PropertyCallbackInfo<Value>& info);
	static void Setter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

	static void toString(const FunctionCallbackInfo<v8::Value>& args);			// 本进程内申请
	static void toNumber(const FunctionCallbackInfo<v8::Value>& args);
	static void toFloat(const FunctionCallbackInfo<v8::Value>& args);
private:
	Isolate* _isolate;
};