#pragma once
#include <Windows.h>
#include <string>

using namespace std;
class guid_until {
public:
	guid_until(_GUID* guid_);
	guid_until(_GUID guid_);

	string toString();

	static guid_until* New(_GUID* guid_);
	static guid_until Instance(_GUID* guid_);
private:
	_GUID guid;

};