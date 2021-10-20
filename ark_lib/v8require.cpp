#include "v8require.h"
#include "file_until.h"
#include "v8context.h"
#include <unordered_set>
#include <string>
#include "v8contextDefine.h"
#include <unordered_map>
#include "v8vm.h"

std::unordered_map<string, Persistent<Value, CopyablePersistentTraits<Value>>> lib;
thread_local v8require* global_v8t = nullptr;
v8require::v8require(Isolate* isolate) :
	_isolate(isolate) {
}

v8require::~v8require() {
}

void require(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	args.GetReturnValue().Set(Undefined(isolate));

	auto str = args[0].As<Value>()->ToString(isolate->GetCurrentContext());
	if (str.IsEmpty())
		return;
	String::Utf8Value file_name(isolate, str.ToLocalChecked());

#ifdef _DEBUG
	printf("import :%s\n", *file_name);
#endif

	// �ӻ����¼�л�ÿ����
	if (lib.find(*file_name) != lib.end()) {
		auto result = lib[*file_name].Get(isolate);
		args.GetReturnValue().Set(result);
		return;
	}

	// ���ļ��л�ÿ����
	FileUntil fp(*file_name);
	if (fp.existing()) {
		auto _context = v8context::New(isolate);
		Context::Scope context_scope(_context);
		{
			MaybeLocal<String> source = String::NewFromUtf8(isolate, (const char*)fp.toMemory());
			if (!source.IsEmpty()) {
				TryCatch trycatch(isolate);
				MaybeLocal<Script> rScript = Script::Compile(_context, source.ToLocalChecked());
				if (!rScript.IsEmpty()) {
					Local<Script> script = rScript.ToLocalChecked();
					MaybeLocal<Value> rResult = script->Run(_context);
					if (!rResult.IsEmpty()) {
						auto result = rResult.ToLocalChecked();
						if (!result.IsEmpty()) {
							// �������Ļ�� module.exports ����,���óɷ���ֵ
							args.GetReturnValue().Set(result);
							// ��¼���������
							Persistent<Value, CopyablePersistentTraits<Value>> pResult(isolate, result);
							lib[*file_name] = pResult;
						}
					}
					else {
						v8vm::outputError(_context, &trycatch);
					}
				}
				else {
					v8vm::outputError(_context, &trycatch);
				}
			}
		}
	}
	else {
		printf("file:%s ->not found\n", *file_name);
	}
}


void v8require::setup(Local<ObjectTemplate> global) {
	// ����ȫ�ֶ���
	global->Set(
		String::NewFromUtf8Literal(_isolate, "require", NewStringType::kNormal),
		FunctionTemplate::New(_isolate, require)
	);
}

// ע���ڴ�й©
v8require* v8require::current(Isolate* isolate) {
	return global_v8t ? global_v8t : new v8require(isolate);
}