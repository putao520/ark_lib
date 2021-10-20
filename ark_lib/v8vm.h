#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>
#include "v8context.h"
#include <chrono>
#include <thread>

using namespace v8;
using namespace std;

extern unique_ptr<Platform> g_default_platform;

/**
* ʵ�� v8 ִ�������Cpp��װ
*/
class v8vm {
public:
	~v8vm();
	v8vm&& script(const char* script);
	v8vm&& load(const char* file);
	v8vm&& from(const char* uri);
	template<typename T>
	T exec() {
		T r;
		if (_script) {
			Isolate::Scope _isolate_scope(_isolate);
			{
				HandleScope handle_scope(_isolate);

				Local<Context> _context = v8context::New(_isolate);

				Context::Scope context_scope(_context);
				{
					MaybeLocal<String> source = String::NewFromUtf8(_isolate, _script);
					if (_script) {
						Local<Value> result;
						TryCatch trycatch(_isolate);

						MaybeLocal<Script> rScript = Script::Compile(_context, source.ToLocalChecked());
						if (!rScript.IsEmpty()) {
							Local<Script> script = rScript.ToLocalChecked();
							MaybeLocal<Value> rResult = script->Run(_context);
							if (!rResult.IsEmpty()) {
								result = rResult.ToLocalChecked();
							}
							else {
								result = trycatch.Exception();
							}
						}
						else {
							result = trycatch.Exception();
							v8vm::outputError(_context, &trycatch);
						}

						// �ַ���
						if constexpr (is_same<decay<T>::type, string>::value) {
							String::Utf8Value utf8(_isolate, result);
							string str = *utf8;
							r = str;
						}
						// �з��� 32λ ����
						else if constexpr (is_signed<T>::value && is_integral<T>::value && sizeof(T) == 4) {
							r = result->Int32Value(_context).ToChecked();

						}
						// �޷��� 32λ ����
						else if constexpr (is_unsigned<T>::value && is_integral<T>::value && sizeof(T) == 4) {
							r = result->Uint32Value(_context).ToChecked();

						}
						// �з��� 64λ ����
						else if constexpr (is_signed<T>::value && is_integral<T>::value && sizeof(T) == 8) {
							r = result->ToBigInt(_context).ToLocalChecked()->Int64Value();

						}
						// �޷��� 64λ ����
						else if constexpr (is_unsigned<T>::value && is_integral<T>::value && sizeof(T) == 8) {
							r = result->ToBigInt(_context).ToLocalChecked()->Uint64Value();
						}
						// double
						else if constexpr (is_floating_point<T>::value && sizeof(T) == 8) {
							r = result->NumberValue(_context).ToChecked();
						}
						// float
						else if constexpr (is_floating_point<T>::value && sizeof(T) == 44) {
							r = static_cast<float>(result->NumberValue(_context).ToChecked());
						}

						// �ȴ�����ִ��
						this->wait();
						// �ȴ��첽����ִ��
					}
				}
			}
		}
		return r;
	}

	v8vm();
	v8vm(const v8vm& that);
	v8vm(const v8vm&& that) noexcept;
	
	// �ȴ���ǰ������������첽������첽�Ƿ�Ϊ��
	v8vm* wait();

	static void outputError(Local<Context> ctx, TryCatch* trycatch);
	static vector<Isolate*> getAllIsolate();
private:
	Isolate* buildIsolate();

	unique_ptr<Isolate::CreateParams> p_create_params;
	Isolate* _isolate;
	const char* _script;
};