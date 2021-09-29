#include "driver_loader.h"
#include "process_adjust.h"
#include <filesystem>
#include <assert.h>

#define SC_HANDLE_ERROR(x) x == NULL || x == (SC_HANDLE)ERROR_INVALID_HANDLE

driver_loader::driver_loader() {
	this->SCM();
}
driver_loader::driver_loader(const char* _uri) {
	this->SCM();
	this->setUri(_uri);
}
driver_loader* driver_loader::InitDevName(const char* lpszDriverName) {
	this->driverName = lpszDriverName;
	return this;
}
string driver_loader::getDevName() {
	string r = this->driverName;
	return r;
}
string driver_loader::getPath() {
	return this->path;
}
driver_loader* driver_loader::setUri(const char* _uri) {
	filesystem::path path(_uri);

	if (path.has_relative_path()) {
		path = absolute(path);
	}

#ifdef _DEBUG
	printf("Current DevFile:%ws\n", path.c_str());
#endif
	
	// assert(filesystem::exists(path));

	this->path = path.string();

	return this;
}
driver_loader::~driver_loader() {
	this->close();
}

void driver_loader::SCM() {
	DWORD dwRtn = 0;
	GsProcess::AdjustPrivilege();
	this->errCode = 0;
	this->loadStatus = 0;
	this->device = NULL;
}

SC_HANDLE driver_loader::openOrCreate() {
	DWORD dwRtn = 0, dwRtn_ = 0;

	if (this->device)
		return this->device;

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (SC_HANDLE_ERROR(hSCManager)) {
		this->errCode = dwRtn = GetLastError();
		if (dwRtn == ERROR_ACCESS_DENIED) {
			MessageBox(NULL, "请使用[管理员]权限运行本程序!", "提示", 64);
			ExitProcess(-1);
		}
		return 0;
	}

WorkPiper:
	SC_HANDLE _device = OpenService(hSCManager, this->driverName, SERVICE_ALL_ACCESS);
	if (SC_HANDLE_ERROR(_device)) {
		this->errCode = dwRtn = GetLastError();
#ifdef _DEBUG
		printf("打开服务错误![%d]\n", dwRtn);
#endif
		if (ERROR_SERVICE_DOES_NOT_EXIST == dwRtn) {
			_device = CreateService(hSCManager,
				this->driverName, //驱动程序的在注册表中的名字  
				this->driverName, // 注册表驱动程序的 DisplayName 值  
				SERVICE_ALL_ACCESS, // 加载驱动程序的访问权限  
				SERVICE_KERNEL_DRIVER,// 表示加载的服务是驱动程序  
				SERVICE_DEMAND_START, // 注册表驱动程序的 Start 值  
				SERVICE_ERROR_IGNORE, // 注册表驱动程序的 ErrorControl 值  
				this->path.c_str(), // 注册表驱动程序的 ImagePath 值  
				NULL,  //GroupOrder HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\GroupOrderList
				NULL,
				NULL,
				NULL,
				NULL);
			if( SC_HANDLE_ERROR(_device ))
				this->errCode = dwRtn = dwRtn_ = GetLastError();
#ifdef _DEBUG
			printf("创建服务错误![%d]\n", dwRtn_);
#endif
		}

		if (ERROR_IO_PENDING == dwRtn) {
			Sleep(1000);
			goto WorkPiper;
		}
	}
	
	CloseServiceHandle(hSCManager);
	return _device;
}

DWORD driver_loader::getError() {
	return this->errCode;
}


DWORD driver_loader::start() {
	DWORD dwRtn = 0;
	if (!this->device)
		return ERROR_SERVICE_DOES_NOT_EXIST;
WorkPiper:
	if (!StartService(this->device, NULL, NULL)) {
		this->errCode = dwRtn = GetLastError();
#ifdef _DEBUG
		printf("开始服务错误![%d]\n", dwRtn);
#endif
		if (dwRtn == ERROR_SERVICE_ALREADY_RUNNING)
			return 0;
		if (dwRtn == ERROR_IO_PENDING) {
			Sleep(1000);
			goto WorkPiper;
		}
	}

#ifdef _DEBUG
	if (dwRtn == 0)
		printf("StartService OK\n");
#endif
	return dwRtn;
}

driver_loader* driver_loader::load() {
	// 文件不存在
	if (!this->path.length()) {
		this->errCode = 404;
		return this;
	}

#ifdef _DEBUG
	printf("CreateService File:%s\n", this->path.c_str());
	printf("CreateService Name:%s\n", this->driverName);
#endif
	this->device = this->openOrCreate();
#ifdef _DEBUG
	printf("device:%p\n", this->device);
#endif
	if (!SC_HANDLE_ERROR(this->device)) {
		if (this->start() == 0) 
			this->loadStatus = 1;
	}
			
	return this;
}

driver_loader* driver_loader::wait() {
	uint32_t max_cnt = 10000;
	uint32_t cur_cnt = 0;
	while (cur_cnt < max_cnt) {
		Sleep(100);
		cur_cnt++;
		if (this->loadStatus == 1) {
			break;
		}
	}
	return this;
}

DWORD driver_loader::remove() {
	if (!this->device)
		return ERROR_SERVICE_DOES_NOT_EXIST;

	DWORD dwRtn = 0;
WorkPiper:
	if (!DeleteService(this->device)) {
		this->errCode = dwRtn = GetLastError();
#ifdef _DEBUG
		printf("删除服务错误![%d]\n", dwRtn);
#endif
		if (dwRtn == ERROR_IO_PENDING) {
			Sleep(1000);
			goto WorkPiper;
		}
	}

	return dwRtn;
}

DWORD driver_loader::stop() {
	SERVICE_STATUS SvrSta;
	DWORD dwRtn = 0;
WorkPiper:
	if (!ControlService(this->device, SERVICE_CONTROL_STOP, &SvrSta)) {
		this->errCode = dwRtn = GetLastError();
#ifdef _DEBUG
		printf("停止服务错误![%d]\n", dwRtn);
#endif
		if (dwRtn == ERROR_IO_PENDING) {
			Sleep(1000);
			goto WorkPiper;
		}
	}

	return dwRtn;
}

driver_loader* driver_loader::unload() {
	this->stop();
	this->remove();
	this->close();
	return this;
}

DWORD driver_loader::send(DWORD controlCode, char* inBuff, DWORD inSize, char* outBuff, DWORD outSize) {
	DWORD dwRtn = 0;
	DWORD rLength = 0;
	
	char tempDriverName[64];
	sprintf_s(tempDriverName, "\\\\.\\%s", this->driverName);
	HANDLE handle = CreateFile(tempDriverName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
	if (!handle)
		return dwRtn;

WorkPiper:
	BOOL cR = DeviceIoControl(handle,
		controlCode,  // 功能号   
		inBuff,   // 输入缓冲，要传递的信息，预先填好   
		inSize,  // 输入缓冲长度   
		outBuff,    // 没有输出缓冲   
		outSize,     // 输出缓冲的长度为0   
		&rLength,   // 返回的长度    
		NULL);
	if (!cR) {
		this->errCode = dwRtn = GetLastError();
#ifdef _DEBUG
		printf("控制服务错误![%d]\n", dwRtn);
#endif
		if (dwRtn == ERROR_IO_PENDING) {
			Sleep(1000);
			goto WorkPiper;
		}
	}
	CloseHandle(handle);
	return cR ? rLength : 0;
}

void driver_loader::close() {
#ifdef _DEBUG
	printf("device handle:%p\n", this->device);
#endif
	if ( this->device )
		while(CloseServiceHandle(this->device));

	this->device = NULL;
}