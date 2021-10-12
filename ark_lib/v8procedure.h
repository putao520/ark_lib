#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>

using namespace v8;
class v8procedure {
public:
	v8procedure(Isolate* isolate);
	~v8procedure();
	void setup(Local<ObjectTemplate>);

	static v8procedure* current(Isolate* isolate);

private:
	Isolate* _isolate;
};