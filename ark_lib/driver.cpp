#include "driver.h"
#include <Psapi.h>
#pragma comment (lib,"Psapi.lib")
#define ARRAY_SIZE 1024

driver::driver() {
	flush();
}

uintptr_t driver::module(string name) {
	uintptr_t handle = load_module[name];
	if (!handle) {
		flush();
		handle = load_module[name];
	}
#ifdef _DEBUG
	printf("%s->%p\n", name.c_str(), (void *)handle);
#endif
	return handle;
}

driver* driver::remove(string name) {
	load_module.erase(name);
	return this;
}

void driver::flush() {
	// 更新 当前系统 驱动列表
	char szDriver[256] = { 0 };	// 驱动文件名
	DWORD cbNeeded = 0; // drivers[] 返回的字节数
	int cDrivers = 0;	// 驱动个数
	if (EnumDeviceDrivers(nullptr, 0, &cbNeeded)) {	// 获得大小
		LPVOID* drivers = (LPVOID *)calloc(1, cbNeeded);
		if (drivers) {
			if (EnumDeviceDrivers(drivers, cbNeeded, &cbNeeded)) {
				cDrivers = cbNeeded / sizeof(LPVOID);	// 驱动个数
				for (int i = 0; i < cDrivers; i++) {
					if (GetDeviceDriverBaseName(drivers[i], szDriver, 256)) {
						string name = szDriver;
						load_module[name] = (uintptr_t)drivers[i];
#ifdef _DEBUG
						printf("[%d]%s:%p\n", i, szDriver, drivers[i]);
#endif
					}
				}
			}
		}
	}
}