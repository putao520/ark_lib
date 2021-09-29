#pragma once
#include "driver_loader.h"
class DriverLoaderService {
public:
	static driver_loader* current();
	static void free();
private:
};