#include "v8memory.h"
#include "ActorService.h"
#include "text_encode.h"

#include "debug_until.h"

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

void v8memory::_new(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		if (args.Length() == 1) {
			auto arg_0 = args[0].As<Value>()->Uint32Value(ctx);
			if (arg_0.IsJust()) {
				uint32_t size = arg_0.ToChecked();

				args.GetReturnValue().Set(BigInt::NewFromUnsigned(isolate, (uint64_t)calloc(1, size)));
				return;
			}
		}
	}
	args.GetReturnValue().SetUndefined();
}

void v8memory::_delete(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		if (args.Length() == 1) {
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (!arg_0.IsEmpty()) {
				uint64_t address = arg_0.ToLocalChecked()->Uint64Value();
				if( address )
					::free((void*)address);
			}
		}
	}
	args.GetReturnValue().SetUndefined();
}


void v8memory::alloc(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		if (args.Length() == 1) {
			auto arg_0 = args[0].As<Value>()->Uint32Value(ctx);
			if ( arg_0.IsJust() ) {
				uint32_t size = arg_0.ToChecked();

				auto result = ActorService::current()
					->push(size)
					->run(ActorService::services()->alloc);

				args.GetReturnValue().Set(BigInt::NewFromUnsigned(isolate, (uint64_t)result));
				return;
			}
		}
	}
	args.GetReturnValue().SetUndefined();
}

void v8memory::free(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		if (args.Length() == 1) {
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (!arg_0.IsEmpty()) {
				uint64_t address = arg_0.ToLocalChecked()->Uint64Value();
				ActorService::current()
					->push(address)
					->run(ActorService::services()->free);
			}
		}
	}
	args.GetReturnValue().SetUndefined();
}

void v8memory::read(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		if (args.Length() == 3) {
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (!arg_0.IsEmpty())
				goto ru;
			uint64_t buffer = arg_0.ToLocalChecked()->Uint64Value();

			auto arg_1 = args[1].As<Value>()->ToBigInt(ctx);
			if (!arg_1.IsEmpty())
				goto ru;
			uint64_t address = arg_1.ToLocalChecked()->Uint64Value();

			auto arg_2 = args[2].As<Value>()->Uint32Value(ctx);
			if (!arg_2.IsNothing())
				goto ru;
			uint32_t size = arg_2.ToChecked();

			auto result = ActorService::current()
				->push(buffer)
				->push(address)
				->push(size)
				->run(ActorService::services()->read);

			args.GetReturnValue().Set(Uint32::New(isolate, (uint32_t)result));
			return;
		}
	}
	ru:
	args.GetReturnValue().SetUndefined();
}

void v8memory::write(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		if (args.Length() == 3) {
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				goto ru;
			uint64_t buffer = arg_0.ToLocalChecked()->Uint64Value();

			auto arg_1 = args[1].As<Value>()->ToBigInt(ctx);
			if (arg_1.IsEmpty())
				goto ru;
			uint64_t address = arg_1.ToLocalChecked()->Uint64Value();

			auto arg_2 = args[2].As<Value>()->Uint32Value(ctx);
			if (arg_2.IsNothing())
				goto ru;
			uint32_t size = arg_2.ToChecked();

			auto result = ActorService::current()
				->push(buffer)
				->push(address)
				->push(size)
				->run(ActorService::services()->write);

			args.GetReturnValue().Set(Uint32::New(isolate, (uint32_t)result));
			return;
		}
	}
ru:
	args.GetReturnValue().SetUndefined();
}

void v8memory::f32(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		switch (args.Length()) {
			case 1: {
				auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
				if( arg_0.IsEmpty())
					break;
				uint64_t address = arg_0.ToLocalChecked()->Uint64Value();
				uint32_t size = 4;
				float v = 0.0f;

				ActorService::current()
					->push(&v)
					->push(address)
					->push(size)
					->run(ActorService::services()->read);

				double r = 0.0;
				r += v;
				args.GetReturnValue().Set(Number::New(isolate, r));
				return;
			}
			case 2: {
				auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
				if (arg_0.IsEmpty())
					break;
				uint64_t address = arg_0.ToLocalChecked()->Uint64Value();

				auto arg_1 = args[1].As<Value>()->NumberValue(ctx);
				if (arg_1.IsNothing())
					break;
				float v = (float)arg_1.ToChecked();

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
	args.GetReturnValue().SetUndefined();
}

void v8memory::f64(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		switch (args.Length()) {
		case 1: {
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				break;
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();
			uint32_t size = 8;
			double v = 0;

			ActorService::current()
				->push(&v)
				->push(address)
				->push(size)
				->run(ActorService::services()->read);

			args.GetReturnValue().Set(Number::New(isolate, v));
			return;
		}
		case 2: {
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				break;
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();

			auto arg_1 = args[1].As<Value>()->NumberValue(ctx);
			if (arg_1.IsNothing())
				break;
			double v = arg_1.ToChecked();

			uint32_t size = 8;
			ActorService::current()
				->push(&v)
				->push(address)
				->push(size)
				->run(ActorService::services()->write);

			break;
		}
		}
	}
	args.GetReturnValue().SetUndefined();
}

void v8memory::u32(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		switch (args.Length()) {
		case 1: {
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				break;
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();

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
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				break;
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();

			auto arg_1 = args[1].As<Value>()->Uint32Value(ctx);
			if (arg_1.IsNothing())
				break;
			uint32_t v = arg_1.ToChecked();

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
	args.GetReturnValue().SetUndefined();
}

void v8memory::i32(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		switch (args.Length()) {
		case 1: {
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				break;
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();
			uint32_t size = 4;
			int32_t v = 0;

			ActorService::current()
				->push(&v)
				->push(address)
				->push(size)
				->run(ActorService::services()->read);

			args.GetReturnValue().Set(Int32::New(isolate, (int32_t)v));
			return;
		}
		case 2: {
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				break;
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();

			auto arg_1 = args[1].As<Value>()->Uint32Value(ctx);
			if (arg_1.IsNothing())
				break;
			int32_t v = arg_1.ToChecked();
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
	args.GetReturnValue().SetUndefined();
}

void v8memory::u64(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		switch (args.Length()) {
		case 1: {
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				break;
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();
			uint32_t size = 8;
			uint64_t v = 0;

			ActorService::current()
				->push(&v)
				->push(address)
				->push(size)
				->run(ActorService::services()->read);

			args.GetReturnValue().Set(BigInt::NewFromUnsigned(isolate, v));
			return;
		}
		case 2: {
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				break;
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();
#ifdef _DEBUG
			printDebug("address:%p", address);
#endif
			auto arg_1 = args[1].As<Value>()->ToBigInt(ctx);
			if (arg_1.IsEmpty())
				break;
			uint64_t v = arg_1.ToLocalChecked()->Uint64Value();
#ifdef _DEBUG
			printDebug("v:%p", v);
#endif
			uint32_t size = 8;
			ActorService::current()
				->push(&v)
				->push(address)
				->push(size)
				->run(ActorService::services()->write);

			break;
		}
		}
	}
	args.GetReturnValue().SetUndefined();
}

void v8memory::i64(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		switch (args.Length()) {
		case 1: {
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				break;
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();
			uint32_t size = 8;
			int64_t v = 0;

			ActorService::current()
				->push(&v)
				->push(address)
				->push(size)
				->run(ActorService::services()->read);

			args.GetReturnValue().Set(BigInt::New(isolate, v));
			return;
		}
		case 2: {
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				break;
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();

			auto arg_1 = args[1].As<Value>()->ToBigInt(ctx);
			if (arg_1.IsEmpty())
				break;
			int64_t v = arg_1.ToLocalChecked()->Int64Value();

			uint32_t size = 8;
			ActorService::current()
				->push(&v)
				->push(address)
				->push(size)
				->run(ActorService::services()->write);

			break;
		}
		}
	}
	args.GetReturnValue().SetUndefined();
}

void v8memory::u8Array(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) {
		if (args.Length() == 2) {
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				goto ru;
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();
#ifdef _DEBUG
			printDebug("address:%p", address);
#endif

			if (args[1]->IsUint8Array()) {	// 写入
				auto buffer_ = args[1].As<Uint8Array>();
				uint32_t size = (uint32_t)buffer_->ByteLength();

				void* buffer = buffer_->Buffer()->GetBackingStore()->Data();
				ActorService::current()
					->push(buffer)
					->push(address)
					->push(size)
					->run(ActorService::services()->write);
			}
			else {	// 读取
				auto arg_1 = args[1].As<Value>()->Uint32Value(ctx);
				if (arg_1.IsNothing())
					goto ru;
				uint32_t size = arg_1.ToChecked();
#ifdef _DEBUG
				printDebug("size:%d", size);
#endif

				auto buffer_ = ArrayBuffer::New(isolate, size);
				auto buffer = buffer_->GetBackingStore().get()->Data();
#ifdef _DEBUG
				printDebug("Array buffer:%p", buffer);
#endif
				
				auto result = ActorService::current()
					->push(buffer)
					->push(address)
					->push(size)
					->run(ActorService::services()->read);
					

				// memcpy(buffer, (void *)address, size);	// 本地测试用
				args.GetReturnValue().Set(Uint8Array::New(buffer_, 0, size));
				return;
			}
		}
	}
ru:
	args.GetReturnValue().SetUndefined();
}

void v8memory::string(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	if (!ctx.IsEmpty()) 
		switch (args.Length()){
		case 1:{		// 读取 utf8/ascii
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				goto ru;
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();
#ifdef _DEBUG
			printDebug("string address:%p", address);
#endif

#ifdef _LOCAL
			uint32_t size = strlen((char *)address);
			char* buffer_ = (char*)calloc(1, (size_t)size + 1);

			if (buffer_)
				memcpy(buffer_, (void*)address, size);

#else
			auto services = ActorService::services();
			// 获得字符串长度
			uint32_t size = (uint32_t)ActorService::current()
				->push(address)
				->run(services->getStringSize);
#ifdef _DEBUG
			printDebug("string size:%d", size);
#endif

			char* buffer_ = (char*)calloc(1, (size_t)size + 1);
			ActorService::current()
				->push(buffer_)
				->push(address)
				->push(size)
				->run(services->read);
#endif

			auto result = String::NewFromUtf8(isolate, buffer_, NewStringType::kNormal);
			::free(buffer_);
			if (result.IsEmpty())
				break;
			args.GetReturnValue().Set(result.ToLocalChecked());
			return;
		}
		case 2: {		// 写入
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				goto ru;
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();
			auto arg_1 = args[1].As<Value>()->ToString(ctx);
			if (arg_1.IsEmpty())
				goto ru;
			String::Utf8Value utf8(isolate, arg_1.ToLocalChecked());
			uint32_t size = (uint32_t)(utf8.length() + 1);
			void* buffer = (void*)*utf8;

#ifdef _LOCAL
			memcpy((void*)address, buffer, size);
#else
			ActorService::current()
				->push(buffer)
				->push(address)
				->push(size)
				->run(ActorService::services()->write);
#endif
			break;
		}
	}
ru:
	args.GetReturnValue().SetUndefined();
}

void v8memory::unicode(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	auto ctx = isolate->GetCurrentContext();
	args.GetReturnValue().SetUndefined();
	if (!ctx.IsEmpty())
		switch (args.Length()) {
		case 1: {		// 读取 unicode
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				return;
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();
#ifdef _DEBUG
			printDebug("unicode address:%p", address);
#endif

#ifdef _LOCAL
			uint32_t size = wcslen((wchar_t*)address) * sizeof(wchar_t);
			wchar_t* buffer_ = (wchar_t*)calloc(1, (size_t)size + 2);

			if (buffer_)
				memcpy(buffer_, (void*)address, size);
#else

			auto services = ActorService::services();
			// 获得字符串长度
			uint32_t size = (uint32_t)ActorService::current()
				->push(address)
				->run(services->getWStringSize);
#ifdef _DEBUG
			printDebug("unicode size:%d", size);
#endif

			char* buffer_ = (char*)calloc(1, (size_t)size + 2);
			if (!buffer_)
				return;

			ActorService::current()
				->push(buffer_)
				->push(address)
				->push(size)
				->run(services->read);
#endif
			
			// unicode => utf8
			char* utf8buffer = TextEncode::unicode2utf8((wchar_t *)buffer_);
			if (utf8buffer) {
				auto result = String::NewFromUtf8(isolate, utf8buffer, NewStringType::kNormal);
				if (!result.IsEmpty()) {
					args.GetReturnValue().Set(result.ToLocalChecked());
				}
				TextEncode::free(utf8buffer);
			}
			::free(buffer_);
			return;
		}
		case 2: {		// 写入
			auto arg_0 = args[0].As<Value>()->ToBigInt(ctx);
			if (arg_0.IsEmpty())
				return;
			uint64_t address = arg_0.ToLocalChecked()->Uint64Value();
			// auto arg_1 = args[1].As<Value>()->ToString(ctx);
			auto arg_1 = args[1].As<String>(); // ->ToString(ctx);
			if (arg_1.IsEmpty())
				return;
			String::Utf8Value utf8(isolate, arg_1);
			uint32_t size = (uint32_t)((utf8.length() + 1) * sizeof(wchar_t));
			void* buffer = (void*)*utf8;
			wchar_t* utf16buffer = (wchar_t *)TextEncode::utf16le((char *)buffer);
			if (utf16buffer) {
#ifdef _LOCAL
				memcpy((void*)address, utf16buffer, size);
#else
				ActorService::current()
					->push(utf16buffer)
					->push(address)
					->push(size)
					->run(ActorService::services()->write);
#endif
				TextEncode::free(utf16buffer);
			}
			break;
		}
		}
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

	// 申请本进程内存
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "_new", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, _new)
	);
	// 释放本进程内存
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "_delete", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, _delete)
	);

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

	// 读取内存
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "read", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, read)
	);
	// 写入内存
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "write", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, write)
	);

	// u32 方法
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "u32", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, u32)
	);

	// i32 方法
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "i32", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, i32)
	);

	// u64 方法
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "u64", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, u64)
	);

	// i64 方法
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "i64", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, i64)
	);

	// f32 方法
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "f32", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, f32)
	);

	// f64 方法
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "f64", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, f64)
	);

	// u8Array 方法
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "u8Array", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, u8Array)
	);

	// string 方法
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "string", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, string)
	);

	// unicode 方法
	template_->Set(
		String::NewFromUtf8Literal(_isolate, "unicode", NewStringType::kInternalized),
		FunctionTemplate::New(_isolate, unicode)
	);
	
	if (gTemplate.IsEmpty())
		gTemplate.Reset(_isolate, template_);
}