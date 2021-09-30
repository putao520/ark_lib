#pragma once
#include <string>

using namespace std;
class TextEncode {
public:
	static char* utf8(char* str);
	static char* utf16le(char* str);

	static char* unicode2utf8(wchar_t* str);

	static void free(void* str);
private:

};