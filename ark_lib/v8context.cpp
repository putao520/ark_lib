#include "v8context.h"
#include "v8inject.h"
#include "v8memory.h"
#include "v8time.h"
#include "v8interval.h"
#include "v8print.h"
#include "v8driver.h"
#include "v8require.h"
#include "v8align.h"
#include "v8procedure.h"

#include "file_until.h"

using namespace std;

#define SETUP_FUNC(type, isolate) type::current(isolate)->setup(global);
#define SETUP_OBJ(type, isolate, name) type::current(isolate)->setup(global, String::NewFromUtf8(isolate, name, NewStringType::kInternalized).ToLocalChecked());
Local<Context> v8context::New(Isolate* isolate) {
	Local<ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
	// Ìî³ä¶ÔÏó
	SETUP_OBJ(v8inject, isolate, "Inject")
	SETUP_OBJ(v8memory, isolate, "Memory")

	// Ìî³äº¯Êý
	SETUP_FUNC(v8time, isolate)
	SETUP_FUNC(v8interval, isolate)
	SETUP_FUNC(v8print, isolate)
	SETUP_FUNC(v8driver, isolate)
	SETUP_FUNC(v8procedure, isolate)
	SETUP_FUNC(v8require, isolate)
	SETUP_FUNC(v8align, isolate)
	
	return Context::New(isolate, NULL, global);
}
