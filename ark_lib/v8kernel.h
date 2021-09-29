#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>

using namespace v8;
using namespace std;

class v8kernel {
public:
	v8kernel(Isolate* isolate);

	static v8kernel* current(Isolate* isolate);
	void setup(Local<ObjectTemplate>, Local<String>);
private:
	Isolate* _isolate;

	Local<ObjectTemplate> MakeTemplate();
};
