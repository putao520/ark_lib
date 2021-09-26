#include "driver_loader.h"
#include "process_adjust.h"
#include <filesystem>

driver_loader::driver_loader() {
	this->_SCM();
}
driver_loader::driver_loader(const char* _uri) {
	this->_SCM();
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
	if (!filesystem::exists(path))
		path = canonical(path);

	this->path = path.string();
#ifdef _DEBUG
	printf("Current DevFile:%s\n", this->path.c_str());
#endif
	this->loadStatus = 1;

	return this;
}
driver_loader::~driver_loader() {
	this->closeFile();
	this->close();
	if (this->hSCManager && this->hSCManager != (SC_HANDLE)ERROR_INVALID_HANDLE)
		CloseServiceHandle(this->hSCManager);
	this->clearContext();
}

void driver_loader::clearContext() {
	if (strlen(this->path.c_str()) > 0) {
		DeleteFile(this->path.c_str());
	}
}
SC_HANDLE driver_loader::_SCM() {
	DWORD dwRtn = 0;
	GsProcess::AdjustPrivilege();
	this->hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (this->hSCManager == NULL || this->hSCManager == (SC_HANDLE)ERROR_INVALID_HANDLE) {
		dwRtn = GetLastError();
		if (dwRtn == ERROR_ACCESS_DENIED) {
			MessageBox(NULL, "请使用[管理员]权限运行本程序!", "提示", 64);
			ExitProcess(-1);
		}
	}
	this->stepCode = 0;
	this->errCode = 0;
	this->loadStatus = 0;
	return this->hSCManager;
}

SC_HANDLE driver_loader::open() {
	if (this->device && this->device != (SC_HANDLE)ERROR_INVALID_HANDLE)
		return this->device;

	int errNo = 0;
	DWORD dwRtn;
	SC_HANDLE hServiceDDK = NULL;

	if (this->errCode == 0 && this->hSCManager) {
		do {
			hServiceDDK = OpenService(this->hSCManager, this->driverName, SERVICE_ALL_ACCESS);
			dwRtn = GetLastError();
			if (dwRtn == 0) {
				break;
			}
#ifdef _DEBUG
			printf("打开服务错误![%d]\n", dwRtn);
#endif
			if (hServiceDDK == NULL || hServiceDDK == (SC_HANDLE)ERROR_INVALID_HANDLE) {
				//如果打开服务也失败，则意味错误
				errNo++;
				Sleep(3000);
			}
			else {
				// _DebugPrint("打开服务错误!", dwRtn);
				this->stepCode = 1;
				this->errCode = dwRtn;
				break;
			}


		} while (errNo < 5 && hServiceDDK != (SC_HANDLE)ERROR_INVALID_HANDLE);
	}
	this->device = hServiceDDK;
	return hServiceDDK;
}

BOOL driver_loader::hasError() {
	return this->errCode > 0;
}

DWORD driver_loader::getError() {
	return this->errCode;
}

DWORD driver_loader::getStep() {
	return this->stepCode;
}

driver_loader* driver_loader::load() {
	SC_HANDLE hServiceDDK = NULL;//NT驱动程序的服务句柄
	int errNo = 0;
	DWORD dwRtn;
	// 文件不存在
	if (!this->path.length()) {
		this->errCode = 404;
		return this;
	}
	this->loadStatus = 2;

#ifdef _DEBUG
	printf("CreateService File:%s\n", this->path.c_str());
#endif
	if (this->errCode == 0 && this->hSCManager && this->hSCManager != (SC_HANDLE)ERROR_INVALID_HANDLE) {
		do {
			//创建驱动所对应的服务
			hServiceDDK = CreateService(this->hSCManager,
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
			dwRtn = GetLastError();
			if (dwRtn == 0) {
				break;
			}
#ifdef _DEBUG
			printf("创建服务错误![%d]\n", dwRtn);
#endif
			//判断服务是否失败
			if (hServiceDDK == NULL || hServiceDDK == INVALID_HANDLE_VALUE) {
				if (dwRtn == ERROR_SERVICE_EXISTS && errNo < 5) {
					errNo++;
					hServiceDDK = this->open();
					break;
				}
				if (dwRtn == ERROR_IO_PENDING && errNo < 5) {//如果服务创建需要等待，那么就等15秒
					errNo++;
					Sleep(3000);
					continue;
				}
				if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS) {
					return this;
				}
				else {
					// _DebugPrint("创建服务错误!", dwRtn);
					this->stepCode = 2;
					this->errCode = dwRtn;
					break;
				}
			}
			else
				break;
		} while (errNo < 5 && dwRtn != 0);
		//开启此项服务
		errNo = 0;

		if (this->errCode == 0 && hServiceDDK) {
			while (!StartService(hServiceDDK, NULL, NULL) && errNo < 5) {
				DWORD dwRtn = GetLastError();
				if (dwRtn == 0) {
					break;
				}
#ifdef _DEBUG
				printf("启动服务错误![%d]\n", dwRtn);
#endif
				if (dwRtn == ERROR_SERVICE_ALREADY_RUNNING) {
					dwRtn = 0;
					break;
				}
				if ((dwRtn == ERROR_IO_PENDING) && errNo < 5) {//如果服务创建需要等待，那么就等15秒
					errNo++;
					Sleep(3000);
					continue;
				}
				else {
					// _DebugPrint("启动服务错误!", dwRtn);
					this->stepCode = 3;
					this->errCode = dwRtn;
					break;
				}
			}
		}
		this->device = hServiceDDK;
	}
	this->loadStatus = 4;
	return this;
}
driver_loader* driver_loader::wait() {
	uint32_t max_cnt = 1000;
	uint32_t cur_cnt = 0;
	while (cur_cnt < max_cnt) {
		Sleep(100);
		cur_cnt++;
		if (this->loadStatus == 4) {
			break;
		}
	}
	return this;
}
driver_loader* driver_loader::unload() {
	SERVICE_STATUS SvrSta;
	DWORD dwRtn = 0;
	DWORD errNo = 0;
	this->device = this->open();

	while (!ControlService(this->device, SERVICE_CONTROL_STOP, &SvrSta) && errNo < 5) {
		dwRtn = GetLastError();
		if (dwRtn == 0) {
			break;
		}
#ifdef _DEBUG
		printf("停止服务错误![%d]\n", dwRtn);
#endif
		if (dwRtn == ERROR_IO_PENDING) {//如果服务创建需要等待，那么就等15秒
			errNo++;
			Sleep(3000);
			continue;
		}
		else {
			// _DebugPrint("停止服务错误!", dwRtn);
			this->stepCode = 4;
			this->errCode = dwRtn;
			break;
		}
	}

	while (!DeleteService(this->device) && errNo < 5) {
		dwRtn = GetLastError();
		if (dwRtn == 0) {
			break;
		}
#ifdef _DEBUG
		printf("删除服务错误![%d]\n", dwRtn);
#endif
		if (dwRtn == ERROR_IO_PENDING) {//如果服务创建需要等待，那么就等15秒
			errNo++;
			Sleep(3000);
			continue;
		}
		else {
			// _DebugPrint("删除服务错误!", dwRtn);
			this->stepCode = 5;
			this->errCode = dwRtn;
			break;
		}
	}
	this->closeFile();
	this->close();

	return this;
}
void driver_loader::closeFile() {
	if (this->handle && this->handle != (HANDLE)ERROR_INVALID_HANDLE)
		CloseHandle(this->handle);
	this->handle = NULL;
}

HANDLE driver_loader::createFile() {

	if (this->handle && this->handle != INVALID_HANDLE_VALUE)
		return this->handle;

	if (!this->driverName)
		return NULL;

	DWORD dwRtn = 0, errNo = 0;
	HANDLE _handle = NULL;
	// \\.\

	char tempDriverName[512];
	strcpy_s(tempDriverName, "\\\\.\\");
	strcat_s(tempDriverName, this->driverName);
	do {
		_handle = CreateFile(tempDriverName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
		if (_handle == INVALID_HANDLE_VALUE) {
			dwRtn = GetLastError();
			if (dwRtn == 0) {
				break;
			}
#ifdef _DEBUG
			printf("创建文件错误![%d]\n", dwRtn);
#endif
			if (dwRtn == ERROR_IO_PENDING && errNo < 5) {//如果服务创建需要等待，那么就等15秒
				errNo++;
				Sleep(3000);
				continue;
			}
			else {
				// _DebugPrint("创建文件错误!", dwRtn);
				this->stepCode = 6;
				this->errCode = dwRtn;
				break;
			}
		}
	} while (errNo < 5 && _handle == INVALID_HANDLE_VALUE);
	this->handle = _handle;
	return _handle;
}


DWORD driver_loader::send(DWORD controlCode, char* inBuff, DWORD inSize, char* outBuff, DWORD outSize) {
	DWORD dwRtn = 0, errNo = 0;
	DWORD rLength = 0;
	this->handle = this->createFile();
	BOOL cR = FALSE;
	/*
	char szText[250];
	sprintf_s(szText, "句柄:%p 错误号:%d\n", this->handle, this->errCode);
	MessageBox(NULL, szText, "提示", 64);
	*/
	while (this->errCode == 0 && !cR && errNo < 5) {
		cR = DeviceIoControl(this->handle,
			controlCode,  // 功能号   
			inBuff,   // 输入缓冲，要传递的信息，预先填好   
			inSize,  // 输入缓冲长度   
			outBuff,    // 没有输出缓冲   
			outSize,     // 输出缓冲的长度为0   
			&rLength,   // 返回的长度    
			NULL);
		dwRtn = GetLastError();
		if (dwRtn == 0) {
			break;
		}
#ifdef _DEBUG
		printf("控制服务错误![%d]\n", dwRtn);
#endif
		if (dwRtn == ERROR_IO_PENDING) {
			errNo++;
			Sleep(3000);
			continue;
		}
		else {
			this->stepCode = 7;
			// _DebugPrint("控制服务错误!", dwRtn);
			/*
			sprintf_s(szText, "控制服务错误!:%d\n", dwRtn);
			MessageBox(NULL, szText, "提示", 64);
			*/
			this->errCode = dwRtn;
			break;
		}
	}
	/*
	sprintf_s(szText, "返回长度!:%d\n", rLength);
	MessageBox(NULL, szText, "提示", 64);
	*/
	return cR ? rLength : 0;
}

void driver_loader::close() {
	if (this->device && this->device != INVALID_HANDLE_VALUE)
		CloseServiceHandle(this->device);
	this->device = NULL;
}