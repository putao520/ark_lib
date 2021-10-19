#include "test.h"
#include "driver_loader.h"
#include "actor_client.h"
#include "v8vm.h"
#include "print_until.h"
#include "debug_until.h"
#include "DriverLoaderService.h"
#include "PdbInfo.h"
#include <restclient-cpp/restclient.h>
#include <restclient-cpp/connection.h>

using namespace v8;
using namespace std;
void load_v8vm() {
	v8vm vm;
	auto r = vm.load("test.js").exec<string>();
	printf("%s\n", r.c_str());

	std::cout << "Hello World!\n";
	
	system("pause");
	DriverLoaderService::free();
}

void test_v8vm() {
	v8vm vm;
	auto r = vm.load("single.js").exec<string>();
	printf("%s\n", r.c_str());

	std::cout << "Hello World!\n";

	system("pause");
	DriverLoaderService::free();
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
	void* eprocess = nullptr;
	char* p = nullptr;
	p += pid;
	uintptr_t r = actor
		->push((void*)p)
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
					->push(9)
					->run(services->process_read);

				printf("return:%s/putao520\n", buffer);
			}
			free(test_buffer);

			printDebug("alloc");
			void* address = (void *)actor->push(0x10)->run(services->alloc);
			printDebug("alloc address:%p\n", address);
			if (address) {
				memset(buffer, 0, 64);
				strcpy_s(buffer, 64, "yuyao1022");

				printDebug("write");
				actor->push(buffer)
					->push(address)
					->push(10)
					->run(services->write);

				memset(buffer, 0, 64);

				printDebug("read");
				actor->push(buffer)
					->push(address)
					->push(10)
					->run(services->read);

				printf("memory return:%s/yuyao1022\n", buffer);

				printDebug("free");
				actor->push(address)->run(services->free);
			}

		}
		
	}
	// Ð¶ÔØÇý¶¯
	system("pause");
	loader->unload();
	delete loader;
}

void debug_pe() {
	// PdbInfo pdbInfo("c:\\windows\\system32\\ntoskrnl.exe");
	auto file = FileUntil::toAbsolute("./ntoskrnl.exe");
	PdbInfo pdbInfo(file.c_str());
	string path = "/download/symbols/" + pdbInfo.getPdbName() + "/" + pdbInfo.getSigned() + "1/" + pdbInfo.getPdbName();
	std::cout << path << std::endl;
	RestClient::init();
	auto client = new RestClient::Connection("http://msdl.microsoft.com");
	client->SetUserAgent("Microsoft-Symbol-Server/10.1710.0.0");
	// RestClient::HeaderFields headers;
	// headers["Accept-Encoding"] = "gzip";
	client->AppendHeader("Accept-Encoding", "gzip");



	auto r = client->get(path);
	if (r.code == 302) {
		r = RestClient::get(r.headers["Location"]);
	}
	if (r.code == 200) {

	}

	RestClient::disable();
}