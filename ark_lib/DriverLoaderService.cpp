#include "DriverLoaderService.h"

driver_loader* global_driver_loader = NULL;
driver_loader* DriverLoaderService::current() {
	if (global_driver_loader)
		return global_driver_loader;

	driver_loader* loader = new driver_loader("ark_sys.sys");
	loader->InitDevName("ArkSys");
	loader->load();

	global_driver_loader = loader;

	return loader;
}

void DriverLoaderService::free() {
	if (!global_driver_loader)
		return;
	driver_loader* loader = global_driver_loader;
	global_driver_loader = NULL;
	loader->unload();
	delete loader;
}