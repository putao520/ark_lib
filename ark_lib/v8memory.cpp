#include "v8memory.h"
#include "ActorService.h"

Global<ObjectTemplate> gTemplate;
v8memory* global_memory = nullptr;

void v8memory::Getter(Local<String> property,
	const PropertyCallbackInfo<Value>& info) {
	auto isolate = info.GetIsolate();
	Local<ObjectTemplate> lTemplate = gTemplate.Get(isolate);
	Local<Object> lObject = lTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
	info.GetReturnValue().Set(lObject);
}

void v8memory::Setter(Local<String> property, Local<Value> value,
	const PropertyCallbackInfo<void>& info) {
}

void v8memory::alloc(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		if (args.Length() == 1) {
			uint32_t size = args[0].As<Value>()->Uint32Value(ctx).ToChecked();

			auto result = ActorService::current()
				->push(size)
				->run(ActorService::services()->alloc);

			args.GetReturnValue().Set(Uint32::New(isolate, (uint32_t)result));
			return;
		}
	}
	args.GetReturnValue().Set(Undefined(isolate));
}

void v8memory::free(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		if (args.Length() == 1) {
			uint64_t address = static_cast<uint64_t>(args[0].As<Value>()->IntegerValue(ctx).ToChecked());
			ActorService::current()
				->push(address)
				->run(ActorService::services()->free);
		}
	}
	args.GetReturnValue().Set(Undefined(isolate));
}

void v8memory::u32(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		switch (args.Length()) {
		case 1: {
			uint64_t address = static_cast<uint64_t>(args[0].As<Value>()->IntegerValue(ctx).ToChecked());
			uint32_t size = 4;
			uint32_t v = 0;
			
			ActorService::current()
				->push(&v)
				->push(address)
				->push(size)
				->run(ActorService::services()->read);
				
			args.GetReturnValue().Set(Uint32::New(isolate, (uint32_t)v));
			return;
		}
		case 2: {
			uint64_t address = static_cast<uint64_t>(args[0].As<Value>()->IntegerValue(ctx).ToChecked());
			uint32_t v = args[1].As<Value>()->Uint32Value(ctx).ToChecked();
			uint32_t size = 4;
			
			ActorService::current()
				->push(&v)
				->push(address)
				->push(size)
				->run(ActorService::services()->write);
				
			break;
		}
		}
	}
	args.GetReturnValue().Set(Undefined(isolate));
}
// --------------------------------------------------------------------------------
v8memory::v8memory(Isolate* isolate) : _isolate(isolate) {}

v8memory* v8memory::current(Isolate* isolate) {
	return global_memory ? global_memory : new v8memory(isolate);
}

void v8memory::setup(Local<ObjectTemplate> parent, Local<String> name) {
	parent->SetAccessor(name, v8memory::Getter, v8memory::Setter);
	// 定义 memory 对象
	Local<ObjectTemplate> template_ = ObjectTemplate::New(_isolate);

	// 申请内存
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "alloc", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, alloc)
	);
	// 释放内存
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "free", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, free)
	);

	// u32 方法
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "u32", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, u32)
	);

	
	if (gTemplate.IsEmpty())
		gTemplate.Reset(_isolate, template_);
}