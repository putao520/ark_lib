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
	// ���� ��ǰϵͳ �����б�
	char szDriver[256] = { 0 };	// �����ļ���
	DWORD cbNeeded = 0; // drivers[] ���ص��ֽ���
	int cDrivers = 0;	// ��������
	if (EnumDeviceDrivers(nullptr, 0, &cbNeeded)) {	// ��ô�С
		LPVOID* drivers = (LPVOID *)calloc(1, cbNeeded);
		if (drivers) {
			if (EnumDeviceDrivers(drivers, cbNeeded, &cbNeeded)) {
				cDrivers = cbNeeded / sizeof(LPVOID);	// ��������
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