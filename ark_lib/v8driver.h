#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>

using namespace v8;
class v8driver {
public:
	v8driver(Isolate* isolate);
	~v8driver();
	void setup(Local<ObjectTemplate>);

	static v8driver* current(Isolate* isolate);

private:
	Isolate* _isolate;
};