#include "text_encode.h"
#include <unicode/ucnv.h>  
#include <unicode/utypes.h>  
#include <unicode/ucsdet.h>  

#define BUF_MAX     4096  

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