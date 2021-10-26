#pragma once
#include <iostream>
#include <filesystem>

using namespace std::filesystem;

enum class TEXT_TYPE
{
	TEXT_ANSI = 0,
	TEXT_UTF8 = 1,
	TEXT_UTF8_BOM = 2,
	TEXT_UTF16_LE = 3,
	TEXT_UTF16_BE = 4,
	TEXT_UNKNOW = 5,
};

class FileUntil {
public:
	

	FileUntil(const char* file);
	FileUntil(const std::string& file);
	// ~FileUntil();
	void* toMemory();
	bool existing();
	void write(const char*, size_t=0);
	void writeString(const std::string& data);
	static std::string toAbsolute(const char* file);

private:
	void init(const char* file);

	path this_path;
	char* buffer;
	size_t size;
};