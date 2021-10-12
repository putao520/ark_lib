#include "v8procedure.h"
#include "ActorService.h"
#include "text_encode.h"
#include <deque>

thread_local v8procedure* global_v8t = nullptr;
v8procedure::v8procedure(Isolate* isolate) :
	_isolate(isolate) {
}

v8procedure::~v8procedure() {
}

void procedure(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	if (args.Length() < 1)
		return;

	auto context = isolate->GetCurrentContext();
	if (context.IsEmpty())
		return;

	// ��õ��õ�ַ
	auto arg_0 = args[0].As<Value>()->ToBigInt(context);
	if (arg_0.IsEmpty())
		return;
	uint64_t address = arg_0.ToLocalChecked()->Uint64Value();

	deque<void*> unicode_ptr_array;
	auto actor = ActorService::current();
	// ��������
	for (int i = 1; i < args.Length(); i++) {
		auto v = args[i];
		// ����
		if (v->IsInt32()) {
			actor->push(v->Int32Value(context).ToChecked());
		}
		// 64λ����
		else if (v->IsBigInt()) {
			actor->push(v->ToBigInt(context).ToLocalChecked()->Uint64Value());
		}
		// utf8�ַ���
		else if (v->IsString()) {
			String::Utf8Value utf8(isolate, v);
			wchar_t* buffer = (wchar_t*)TextEncode::utf16le(*utf8);
			if (buffer) {
				actor->push(buffer);
				unicode_ptr_array.push_back(buffer);
			}
		}
		// uint8array����
		else if (v->IsUint8Array()) {
			auto buffer_ = args[1].As<Uint8Array>();
			void* buffer = buffer_->Buffer()->GetBackingStore()->Data();
			actor->push(buffer);
		}
	}
	// ���ú���
	args.GetReturnValue().Set(BigInt::New(isolate, actor->run((void*)address)));

	// �ͷ����� wchart_ �ڴ�
	for (void* pb : unicode_ptr_array) {
		if(pb)
			TextEncode::free(pb);
	}
}

void v8procedure::setup(Local<ObjectTemplate> global) {
	// ����ȫ�ֶ���
	global->Set(
		String::NewFromUtf8Literal(_isolate, "procedure", NewStringType::kNormal),
		FunctionTemplate::New(_isolate, procedure)
	);
}

// ע���ڴ�й©
v8procedure* v8procedure::current(Isolate* isolate) {
	return global_v8t ? global_v8t : new v8procedure(isolate);
}