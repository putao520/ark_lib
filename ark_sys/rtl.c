#include "rtl.h"

void _printf(const char* _Format, ...) {
	va_list ptr;
	va_start(ptr, _Format);
#ifdef WIN_KERNEL
	/*
	#define DPFLTR_ERROR_LEVEL 0
	#define DPFLTR_WARNING_LEVEL 1
	#define DPFLTR_TRACE_LEVEL 2
	#define DPFLTR_INFO_LEVEL 3
	#define DPFLTR_MASK 0x80000000
	*/
	vDbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, _Format, ptr);
#else
	vprintf(_Format, ptr);
#endif // WIN_KERNEL
	va_end(ptr);
}


void _GetTickCount(PULONG msec) {
#ifdef WIN_KERNEL
	LARGE_INTEGER tick_count;
	ULONG myInc = KeQueryTimeIncrement();
	KeQueryTickCount(&tick_count);
	tick_count.QuadPart *= myInc;
	tick_count.QuadPart /= 10000;
	*msec = tick_count.LowPart;
#else
	* msec = GetTickCount();
#endif
}

void _GetTickCount64(PULONGLONG msec) {
#ifdef WIN_KERNEL
	LARGE_INTEGER tick_count;
	ULONG myInc = KeQueryTimeIncrement();
	KeQueryTickCount(&tick_count);
	tick_count.QuadPart *= myInc;
	tick_count.QuadPart /= 10000;
	*msec = tick_count.QuadPart;
#else
	* msec = GetTickCount64();
#endif
}

ULONG _GetCpuCount() {
#ifdef WIN_KERNEL
	return KeQueryMaximumProcessorCount();
#else
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	return sysInfo.dwNumberOfProcessors;
#endif 
}

ULONG _GetCpuNum() {
#ifdef WIN_KERNEL
	return KeGetCurrentProcessorNumber();
#else
	return GetCurrentProcessorNumber();
#endif 
}

void _Sleep(unsigned long dwMilliseconds) {
#ifdef WIN_KERNEL
	LARGE_INTEGER my_interval;
	my_interval.QuadPart = DELAY_ONE_MILLISECOND;
	my_interval.QuadPart *= dwMilliseconds;
	KeDelayExecutionThread(KernelMode, 0, &my_interval);
#else
	Sleep(dwMilliseconds);
#endif
}

int _sprintf(char* str, size_t size, const char* format, ...) {
	va_list va;
	va_start(va, format);
#ifdef WIN_KERNEL
	NTSTATUS s = RtlStringCchPrintfA(str, size, format, va);
	int r = NT_SUCCESS(s) ? (int)size : -1;
#else
	int r = vsnprintf(str, size, format, va);
#endif
	va_end(va);
	return r;
}

void _QueryLocalTime(TIME_FIELDS* out) {
#ifdef WIN_KERNEL
	LARGE_INTEGER CurrentTime, LocalTime;
	KeQuerySystemTime(&CurrentTime);
	ExSystemTimeToLocalTime(&CurrentTime, &LocalTime);
	RtlTimeToTimeFields(&LocalTime, out);
#else
	SYSTEMTIME t;
	GetLocalTime(&t);
	out->Year = t.wYear;
	out->Month = t.wMonth;
	out->Day = t.wDay;
	out->Hour = t.wHour;
	out->Minute = t.wMinute;
	out->Second = t.wSecond;
	out->Milliseconds = t.wMilliseconds;
	out->Weekday = t.wDayOfWeek;
#endif
}

void* _malloc(size_t size) {
#ifdef WIN_KERNEL
	void* r = ExAllocatePoolWithTag(NonPagedPool, size, 0);
	if(r)
		memset(r, 0, size);
	return r;
#else
	return calloc(1, size);
#endif
}


void _free(void* pointer) {
#ifdef WIN_KERNEL
	ExFreePool(pointer);
#else
	free(pointer);
#endif
}

unsigned int _strlen(char* cs, unsigned int maxlength) {

	unsigned int i = 0;
	for (; i < maxlength; i++) {
		if (!*(unsigned char*)cs)
			return i;
		cs++;
	}
	return i;
}

#ifdef _WIN64
__int64 _atoi(const unsigned char* str) {
	__int64 result = 0; //8个字节长度
	__int64 flag = 1;//默认正数

	__int64 max_int = LLONG_MAX, min_int = LLONG_MIN;
#else
__int32 _atoi(const char* str) {
	__int32 result = 0; //8个字节长度
	__int32 flag = 1;//默认正数

	__int32 max_int = INT_MAX, min_int = INT_MIN;
#endif // _WIN64

	//判断指针是否为空
	if (str == NULL)
		return 0;
	//跳过前面的空白字符
	while (isspace(*str)) {
		++str;
	}
	/*
	*判断正负号
	*/
	if (*str == '-') {
		flag = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	//把数字字符逐个转换成整数，并校验溢出,溢出返回0
	while (*str <= '9' && *str >= '0') {
		result = result * 10 + *str - '0';
		if (flag == 1) { //校验是否正溢出
			if (result > max_int)
				return 0;
		}
		else { //校验负溢出
			if (-result < min_int)
				return 0;
		}
		str++;
	}
	return flag * result;
}

#ifdef _WIN64
unsigned __int64 _atoui(const char* str) {
	unsigned __int64 result = 0; //8个字节长度
	unsigned __int64 flag = 1;//默认正数

	unsigned __int64 max_int = ULLONG_MAX;
#else
unsigned __int32 _atoui(const char* str) {
	__int32 result = 0; //8个字节长度
	__int32 flag = 1;//默认正数

	__int32 max_int = UINT_MAX;
#endif // _WIN64

	//判断指针是否为空
	if (str == NULL)
		return 0;

	//跳过前面的空白字符
	while (isspace(*str)) {
		++str;
	}

	//把数字字符逐个转换成整数，并校验溢出,溢出返回0
	while (*str <= '9' && *str >= '0') {
		result = result * 10 + *str - '0';
		if (flag == 1) { //校验是否正溢出
			if (result > max_int)
				return 0;
		}
		str++;
	}
	return flag * result;
}

#ifndef WIN_KERNEL

#ifdef _WIN64

char* g_itoa(__int64 num, int radix) {
	unsigned __int64 unum; /* 中间变量 */

#else

char* g_itoa(__int32 num, int radix) {
	unsigned __int32 unum; /* 中间变量 */
#endif

	char* str = (char *)_malloc(60);
	memset(str, 0, 60);
	/* 索引表 */
	char index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i = 0, j, k;
	/* 确定unum的值 */
	if (radix == 10 && num < 0) /* 十进制负数 */
	{
		unum = (unsigned)-num;
		str[i++] = '-';
	}
	else unum = (unsigned)num; /* 其它情况 */
	/* 逆序 */
	do
	{
		str[i++] = index[unum % (unsigned)radix];
		unum /= radix;
	} while (unum);
	str[i] = '\0';
	/* 转换 */
	if (str[0] == '-') k = 1; /* 十进制负数 */
	else k = 0;
	/* 将原来的“/2”改为“/2.0”，保证当num在16~255之间，radix等于16时，也能得到正确结果 */
	char temp;
	for (j = k; j <= ((double)i - (double)k - 1) / 2.0; j++)
	{
		temp = str[j];
		str[j] = str[i - j - 1];
		str[i - j - 1] = temp;
	}

	return str;
}

#ifdef _WIN64

char* g_uitoa(unsigned __int64 num, int radix) {
	unsigned __int64 unum;

#else

char* g_uitoa(unsigned __int32 num, int radix) {
	unsigned __int32 unum;

#endif

	char* str = (char *)_malloc(60);
	/* 索引表 */
	char index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i = 0, j, k;
	/* 确定unum的值 */
	unum = (unsigned)num; /* 其它情况 */
	/* 逆序 */
	do
	{
		str[i++] = index[unum % (unsigned)radix];
		unum /= radix;
	} while (unum);
	str[i] = '\0';
	/* 转换 */
	k = 0;
	/* 将原来的“/2”改为“/2.0”，保证当num在16~255之间，radix等于16时，也能得到正确结果 */
	char temp;
	for (j = k; j <= ((double)i - (double)k - 1) / 2.0; j++)
	{
		temp = str[j];
		str[j] = str[i - j - 1];
		str[i - j - 1] = temp;
	}

	return str;
}

double _atof(const char* s)//字符型转浮点型
{
	int i = 0;
	int k = 1;
	double d;
	double n = 0, m = 0;
	BOOL flag = TRUE;
	if (*s == '-')//处理符号
	{
		flag = FALSE;
		i++;
	}
	else
	{
		flag = TRUE;
	}
	while (*(s + i) != '\0')
	{
		while (*(s + i) >= '0' && *(s + i) <= '9')//处理整数部分
		{
			n = n * 10 + (*(s + i) - '0');
			i++;
		}
		i++;
		while (*(s + i) >= '0' && *(s + i) <= '9')//处理小数部分
		{
			m = m * 10 + (*(s + i) - '0');
			k *= 10;
			i++;
		}
	}
	if (flag)
		d = n + m / k;
	else
		d = -1 * (n + m / k);
	return d;
}
#endif