#include "text_encode.h"
#include <unicode/ucnv.h>  
#include <unicode/utypes.h>  
#include <unicode/ucsdet.h>  

#define BUF_MAX     4096  

/*
 * data��    ��������� ��Ҫ̽����ַ���
 * len��     ��������� ̽���ַ�������
 * detected  ���������� ̽������п��ܵ��ַ���������, ��������Ҫ�ͷŸ��ֶ�
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

    ucsdet_setText(csd, data, len, &status);
    if (status != U_ZERO_ERROR)
        return false;

    csm = ucsdet_detectAll(csd, &matchCount, &status);
    if (status != U_ZERO_ERROR)
        return false;

#if 0 //��ӡ��̽��Ŀ��ܵı���  
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
        *detected = _strdup(ucsdet_getName(csm[0], &status)); //�������ڴ棬 ��Ҫ�ͷ�  
        if (status != U_ZERO_ERROR)
            return false;
    }

    // printf("charset = %s\n", *detected);

    ucsdet_close(csd);
    return true;
}


/*
 * toConverterName,      ת������ַ�����
 * fromConverterName,    ת��ǰ���ַ�����
 * target,               �洢ת������ַ����� ��������
 * targetCapacity,       �洢������target�Ĵ�С
 * source��              ��Ҫת�����ַ���
 * sourceLength,         source�Ĵ�С
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
    char* target = str;

    if (detectTextEncoding(buffer, size, &detected)) {
        target = new char[size * 2];
        memset(target, 0, size * 2);
        if (convert("UTF-8", detected, target, size * 2, buffer, size) != U_ZERO_ERROR) {
            delete[] target;
            target = str;
        }
        else {
            delete[] str;
        }
        delete[] detected;
    }

    return target;
}