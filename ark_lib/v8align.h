#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>

using namespace v8;
class v8align {
public:
	v8align(Isolate* isolate);
	~v8align();
	void setup(Local<ObjectTemplate>);

	static v8align* current(Isolate* isolate);

private:
	Isolate* _isolate;
};