#pragma once
#include <Windows.h>
#include <string>


class guid_until {
public:
	guid_until(_GUID* guid_);
	guid_until(_GUID guid_);

	std::string toString();

	static guid_until* New(_GUID* guid_);
	static guid_until Instance(_GUID* guid_);

	static std::string getRandomUUID(size_t length = 16);
private:
	_GUID guid;

};