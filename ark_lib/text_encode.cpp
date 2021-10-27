#include "text_encode.h"
#include <unicode/ucnv.h>  
#include <unicode/utypes.h>  
#include <unicode/ucsdet.h>  
#include <windows.h>
#define BUF_MAX     4096  

using namespace std;
/*
 * data    传入参数， 需要探测的字符串
 * len，    传入参数， 探测字符串长度
 * detected  传出参数， 探测的最有可能的字符编码名称, 调用者需要释放该字段
**/
bool detectTextEncoding(const char* data, size_t len, char** detected)
{
    UCharsetDetector* csd;
    const UCharsetMatch** csm;
    // int32_t match, matchCount = 0;
    int32_t matchCount = 0;

    UErrorCode status = U_ZERO_ERROR;

    csd = ucsdet_open(&status);
    if (status != U_ZERO_ERROR)
        return false;

    ucsdet_setText(csd, data, (int32_t)len, &status);
    if (status != U_ZERO_ERROR)
        return false;

    csm = ucsdet_detectAll(csd, &matchCount, &status);
    if (status != U_ZERO_ERROR)
        return false;

#if 0 //打印出探测的可能的编码  
    for (match = 0; match < matchCount; match += 1)
    {
        const char* name = ucsdet_getName(csm[match], &status);
        const char* lang = ucsdet_getLanguage(csm[match], &status);
        int32_t confidence = ucsdet_getConfidence(csm[match], &status);

        if (lang == NULL || strlen(lang) == 0)
            lang = "**";

        // printf("%s (%s) %d\n", name, lang, confidence);
    }
#endif  

    if (matchCount > 0)
    {
        *detected = _strdup(ucsdet_getName(csm[0], &status)); //分配了内存， 需要释放  
        if (status != U_ZERO_ERROR)
            return false;
    }

    // printf("charset = %s\n", *detected);

    ucsdet_close(csd);
    return true;
}


/*
 * toConverterName,      转换后的字符编码
 * fromConverterName,    转换前的字符编码
 * target,               存储转换后的字符串， 传出参数
 * targetCapacity,       存储容量，target的大小
 * source，              需要转换的字符串
 * sourceLength,         source的大小
**/
int convert(const char* toConverterName, const char* fromConverterName,
    char* target, int32_t targetCapacity, const char* source, int32_t sourceLength)
{
    UErrorCode error = U_ZERO_ERROR;
    ucnv_convert(toConverterName, fromConverterName, target, targetCapacity, source, sourceLength, &error);

    return error;
}

char* TextEncode::utf8(char * str) {
    char* detected = NULL;
    string text = str;
    size_t size = text.length();
    const char* buffer = text.c_str();
    char* target = nullptr;
    if (detectTextEncoding(buffer, size, &detected)) {
        target = new char[size * 2];
        memset(target, 0, size * 2);
        if (convert("UTF-8", detected, target, (int32_t)(size * 2), buffer, (int32_t)size) != U_ZERO_ERROR) {
            delete[] target;
            target = str;
        }
        delete[] detected;
    }
    return target;
}

char* TextEncode::utf16le(char* str) {
	char* detected = NULL;
	string text = str;
	size_t size = text.length();
	const char* buffer = text.c_str();
	char* target = nullptr;
	if (detectTextEncoding(buffer, size, &detected)) {
		target = new char[size * 4];
		memset(target, 0, size * 4);
		if (convert("UTF-16LE", detected, target, (int32_t)(size * 4), buffer, (int32_t)size) != U_ZERO_ERROR) {
			delete[] target;
			target = str;
		}
		delete[] detected;
	}

	return target;
}

void TextEncode::free(void* str) {
    if( str )
        delete[] str;
}

char* TextEncode::unicode2utf8(wchar_t* str) {
	char* detected = NULL;
	wstring text = str;
	size_t size = text.length()*sizeof(wchar_t);
	const wchar_t* buffer = text.c_str();
	char* target = nullptr;
	if (detectTextEncoding((const char *)buffer, size, &detected)) {
		target = new char[size * 2];
		memset(target, 0, size * 2);
		if (convert("UTF-8", detected, target, (int32_t)(size * 2), (char*)buffer, (int32_t)size) != U_ZERO_ERROR) {
			delete[] target;
			target = (char *)str;
		}
		delete[] detected;
	}
	return target;
}
/*
std::string TextEncode::UnicodeToAscii(const wchar_t* str)
{
	int	iTextLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	std::vector<char> vecText(iTextLen, '\0');
	::WideCharToMultiByte(CP_ACP, 0, str, -1, &(vecText[0]), iTextLen, NULL, NULL);

	std::string strText = &(vecText[0]);

	return strText;
}

std::string TextEncode::UTF8ToString(const char* utf8Data)
{
	//先将UTF-8转换成Unicode
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::wstring wString = conv.from_bytes(utf8Data);
	//在转换成string
	return UnicodeToAscii(wString.c_str());
}
*/


wchar_t* TextEncode::utf8_to_unicode(char* strUTF8A) {
	int nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8A, -1, NULL, NULL);
	WCHAR* wszBuffer = new WCHAR[(size_t)nLen + 1];
	nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8A, -1, wszBuffer, nLen);
	return wszBuffer;
}

char* TextEncode::utf8_to_ansi(char* strUTF8A) {
	wchar_t* wszBuffer = utf8_to_unicode(strUTF8A);
	char* result = unicode_to_ansi(wszBuffer);
	delete[] wszBuffer;
	return result;
}

char* TextEncode::unicode_to_utf8(wchar_t* strUnicode) {
	int nLen = WideCharToMultiByte(CP_UTF8, NULL, strUnicode, -1, NULL, NULL, NULL, NULL);
	CHAR* szBuffer = new CHAR[(size_t)nLen + 1];
	nLen = WideCharToMultiByte(CP_UTF8, NULL, strUnicode, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;
	return szBuffer;
}
char* TextEncode::ansi_to_utf8(char* strAnsiA) {
	wchar_t* wszBuffer = ansi_to_unicode(strAnsiA);
	char* result = unicode_to_utf8(wszBuffer);
	delete[] wszBuffer;
	return result;
}
wchar_t* TextEncode::ansi_to_unicode(char* strAnsi) {
	int length = MultiByteToWideChar(CP_ACP, 0, strAnsi, (int)strlen(strAnsi) + 1, NULL, 0);//该函数映射一个字符串到一个宽字符（unicode）的字符串
	WCHAR* wszBuffer = new WCHAR[(size_t)length + 1];
	wszBuffer[length] = 0;
	MultiByteToWideChar(CP_ACP, 0, strAnsi, (int)strlen(strAnsi) + 1, wszBuffer, length);
	return wszBuffer;
}
char* TextEncode::unicode_to_ansi(wchar_t* strUnicode) {
	int length = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	char* szBuffer = new char[(size_t)length + 1];
	szBuffer[length] = 0;
	WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, szBuffer, length, NULL, NULL);
	return szBuffer;
}

std::string TextEncode::string_to_utf8(const std::string& str){
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[(size_t)nwLen + 1];//一定要加1，不然会出现尾巴
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[(size_t)nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

std::string TextEncode::utf8_to_string(const std::string& str){
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[(size_t)nwLen + 1];//一定要加1，不然会出现尾巴
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[(size_t)nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;
}