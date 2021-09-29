#pragma once
#include <windows.h>
#include <unordered_map>

using namespace std;
class driver {
public:
	driver();

	uintptr_t module(string name);
	driver* remove(string name);
private:
	void flush();
	unordered_map<string, uintptr_t> load_module;
};