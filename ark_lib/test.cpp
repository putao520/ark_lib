#include "test.h"
#include "driver_loader.h"
#include "actor_client.h"
#include "v8vm.h"
#include "print_until.h"

using namespace v8;
using namespace std;
void load_v8vm() {
	v8vm vm;
	auto r = vm.load("test.js").exec<string>();
	printf("%s\n", r.c_str());

	std::cout << "Hello World!\n";
	this_thread::sleep_for(std::chrono::seconds(3000));
}

void load_kernel_module() {
	driver* load_driver = new driver();
	
	uintptr_t handle = load_driver->module("hal.dll");

	printf("hal.dll=>%p\n", (void*)handle);

	delete load_driver;
}

void* get_eprocess(actor_client* actor, DWORD pid) {
	InternalServices* services = actor->services();

	// ·½±ã²âÊÔ£¬Ó²±àÂëÁËº¯ÊýºÍÆ«ÒÆ
	// void* pGetPeb = services->process_peb;
	void* rvaPsLookupProcessByProcessId = (void *)0x665da0;
	void* eprocess = NULL;
	uintptr_t r = actor
		->push((void*)pid)
		->push((void*)&eprocess)
		->runAsRva("ntoskrnl.exe", rvaPsLookupProcessByProcessId);

#ifdef _DEBUG
	printf("PsLookupProcessByProcessId->Status:%p\n", (void *)r);
#endif

	if (r)
		return nullptr;

	return eprocess;
}

void* enum_process(actor_client* actor, void* eprocess) {
	InternalServices* services = actor->services();

	// ·½±ã²âÊÔ£¬Ó²±àÂëÁËº¯ÊýºÍÆ«ÒÆ
	void* pGetPeb = services->process_peb;
	uintptr_t rvaActiveProcessLink = 0x0;
	uintptr_t rvaPid = 0x0;
	uintptr_t rvaName = 0x0;



	return eprocess;
}

void load_driver() {
	driver_loader* loader = new driver_loader("ark_sys.sys");
	loader->InitDevName("ArkSys");
	loader->load();

	actor_client* actor = actor_client::New(loader);
	actor->connect();

	// µ÷ÓÃÄÚºË·½·¨
	system("pause");

	InternalServices* services = actor->services();

	printf("services:%p\n", services);

	if (services) {
		char buffer[64] = { 0 };

		char* test_buffer = (char*)calloc(1, 0x100);
		if (test_buffer) {
			memcpy(test_buffer, "putao520", 9);
			// ²âÊÔ process read 
			void* eprocess = get_eprocess(actor, GetCurrentProcessId());
			printf("EPROCESS:%p\n", eprocess);
			if (eprocess) {
				uintptr_t r = actor
					->push(eprocess)
					->push(buffer)
					->push(test_buffer)
					->push((void*)9)
					->run(services->process_read);

				printf("return:%s/putao520\n", buffer);
			}
			free(test_buffer);
		}
		
	}
	// Ð¶ÔØÇý¶¯
	system("pause");
	loader->unload();
	delete loader;
}