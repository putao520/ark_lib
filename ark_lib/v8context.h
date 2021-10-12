#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>

using namespace v8;
using namespace std;

class v8context {
public:
	static Local<Context> New(Isolate* isolate);
private:
	Isolate* isolate;
};