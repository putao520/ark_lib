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

	// 驱动加载状态 0:未加载，1：已加载
	uint32_t loadStatus = 0;

	// 驱动句柄
	SC_HANDLE device = NULL;
	// 文件句柄
	DWORD errCode = 0;

	void SCM();
	SC_HANDLE openOrCreate();

	DWORD start();
	DWORD remove();
	DWORD stop();

	void close();
public:
	// 构造函数,输入sys文件uri
	driver_loader();
	driver_loader(const char* _uri);
	~driver_loader();
	driver_loader* InitDevName(const char* lpszDriverName);
	driver_loader* setUri(const char* _uri);
	driver_loader* wait();
	// 清除上下文(仅当下载时有效),删除下载文件
	string getPath();
	string getDevName();
	DWORD getError();

	// 等待结束
	driver_loader* load();
	DWORD send(DWORD controlCode, char* inBuff, DWORD inSize, char* outBuff, DWORD outSize);
	driver_loader* unload();
};