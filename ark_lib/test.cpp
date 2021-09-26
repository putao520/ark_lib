#include "test.h"
#include "driver_loader.h"
#include "actor_client.h"
#include "v8vm.h"

using namespace v8;
using namespace std;
void load_v8vm() {
	v8vm vm;
	auto r = vm.load("p.js").exec<string>();
	printf("%s\n", r.c_str());

	std::cout << "Hello World!\n";
	this_thread::sleep_for(std::chrono::seconds(3000));
}

void load_driver() {
	driver_loader* loader = new driver_loader("ark_sys.sys");
	loader->InitDevName("ArkSys");
	loader->load();

	actor_client* actor = actor_client::New(loader);
	actor->connect();

	// 调用内核方法

	delete loader;
}