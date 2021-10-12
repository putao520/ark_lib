#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>

using namespace v8;
class v8require {
public:
	v8require(Isolate* isolate);
	~v8require();
	void setup(Local<ObjectTemplate>);

	static v8require* current(Isolate* isolate);

private:
	Isolate* _isolate;
};