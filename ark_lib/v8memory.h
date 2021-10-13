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

	
	static void _new(const FunctionCallbackInfo<v8::Value>& args);			// 本进程内申请
	static void _delete(const FunctionCallbackInfo<v8::Value>& args);		// 本进程内释放

	static void alloc(const FunctionCallbackInfo<v8::Value>& args);
	static void free(const FunctionCallbackInfo<v8::Value>& args);

	static void read(const FunctionCallbackInfo<v8::Value>& args);
	static void write(const FunctionCallbackInfo<v8::Value>& args);

	static void u32(const FunctionCallbackInfo<v8::Value>& args);
	static void i32(const FunctionCallbackInfo<v8::Value>& args);
	static void u64(const FunctionCallbackInfo<v8::Value>& args);
	static void i64(const FunctionCallbackInfo<v8::Value>& args);

	static void f32(const FunctionCallbackInfo<v8::Value>& args);
	static void f64(const FunctionCallbackInfo<v8::Value>& args);

	static void buffer(const FunctionCallbackInfo<v8::Value>& args);
	static void string(const FunctionCallbackInfo<v8::Value>& args);
	static void unicode(const FunctionCallbackInfo<v8::Value>& args);
private:
	Isolate* _isolate;

};
