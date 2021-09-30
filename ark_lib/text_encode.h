#pragma once
#include <string> 
// #include <locale> 
// #include <codecvt> 
// #include <vector>
// #include <stdio.h>
// #include <windows.h>

class TextEncode {
public:
	static char* utf8(char* str);
	static char* utf16le(char* str);

	static char* unicode2utf8(wchar_t* str);

	static void free(void* str);

	/*
	static std::string UnicodeToAscii(const wchar_t*);
	static std::string UTF8ToString(const char* utf8Data);
	*/
private:

};