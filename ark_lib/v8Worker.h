#pragma once
#include "v8vm.h"
class v8Worker{
public:
	v8Worker();
	~v8Worker();

private:
	v8vm vm;
};

