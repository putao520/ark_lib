#pragma once
#include <Windows.h>
#include <string>

using namespace std;

class driver_loader {
private:
	// 文件地址,如果是http开头的path,尝试自动下载
	string path;
	const char* driverName = NULL;
	// 与驱动通讯是否使用加密模式
	bool isSecTranport = false;
	// 驱动加载状态 0:未加载，1：下载中，2：加载中 4：结束
	uint32_t loadStatus = 0;

	SC_HANDLE device = NULL;
	HANDLE handle = NULL;
	SC_HANDLE hSCManager = NULL;
	DWORD errCode = 0;

	DWORD stepCode = 0;

	SC_HANDLE _SCM();
	SC_HANDLE open();

	HANDLE createFile();
	void closeFile();

	void close();
public:
	// 构造函数,输入sys文件uri
	driver_loader();
	driver_loader(const char* _uri);
	~driver_loader();
	driver_loader* InitDevName(const char* lpszDriverName);
	driver_loader* setUri(const char* _uri);
	// 清除上下文(仅当下载时有效),删除下载文件
	void clearContext();
	driver_loader* load();
	string getPath();
	string getDevName();
	BOOL hasError();
	DWORD getError();
	DWORD getStep();
	DWORD send(DWORD controlCode, char* inBuff, DWORD inSize, char* outBuff, DWORD outSize);
	// 等待结束
	driver_loader* wait();
	driver_loader* unload();
};