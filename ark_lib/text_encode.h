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
	static char* utf8_to_ansi(char*);
	static char* ansi_to_utf8(char*);

	static wchar_t* utf8_to_unicode(char*);
	static char* unicode_to_utf8(wchar_t*);

	static wchar_t* ansi_to_unicode(char*);
	static char* unicode_to_ansi(wchar_t*);

	static std::string string_to_utf8(const std::string& str);
	static std::string utf8_to_string(const std::string& str);
private:

};