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
			MessageBox(NULL, "��ʹ��[����Ա]Ȩ�����б�����!", "��ʾ", 64);
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
			printf("�򿪷������![%d]\n", dwRtn);
#endif
			if (hServiceDDK == NULL || hServiceDDK == (SC_HANDLE)ERROR_INVALID_HANDLE) {
				//����򿪷���Ҳʧ�ܣ�����ζ����
				errNo++;
				Sleep(3000);
			}
			else {
				// _DebugPrint("�򿪷������!", dwRtn);
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
	SC_HANDLE hServiceDDK = NULL;//NT��������ķ�����
	int errNo = 0;
	DWORD dwRtn;
	// �ļ�������
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
			//������������Ӧ�ķ���
			hServiceDDK = CreateService(this->hSCManager,
				this->driverName, //�����������ע����е�����  
				this->driverName, // ע������������ DisplayName ֵ  
				SERVICE_ALL_ACCESS, // ������������ķ���Ȩ��  
				SERVICE_KERNEL_DRIVER,// ��ʾ���صķ�������������  
				SERVICE_DEMAND_START, // ע������������ Start ֵ  
				SERVICE_ERROR_IGNORE, // ע������������ ErrorControl ֵ  
				this->path.c_str(), // ע������������ ImagePath ֵ  
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
			printf("�����������![%d]\n", dwRtn);
#endif
			//�жϷ����Ƿ�ʧ��
			if (hServiceDDK == NULL || hServiceDDK == INVALID_HANDLE_VALUE) {
				if (dwRtn == ERROR_SERVICE_EXISTS && errNo < 5) {
					errNo++;
					hServiceDDK = this->open();
					break;
				}
				if (dwRtn == ERROR_IO_PENDING && errNo < 5) {//������񴴽���Ҫ�ȴ�����ô�͵�15��
					errNo++;
					Sleep(3000);
					continue;
				}
				if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS) {
					return this;
				}
				else {
					// _DebugPrint("�����������!", dwRtn);
					this->stepCode = 2;
					this->errCode = dwRtn;
					break;
				}
			}
			else
				break;
		} while (errNo < 5 && dwRtn != 0);
		//�����������
		errNo = 0;

		if (this->errCode == 0 && hServiceDDK) {
			while (!StartService(hServiceDDK, NULL, NULL) && errNo < 5) {
				DWORD dwRtn = GetLastError();
				if (dwRtn == 0) {
					break;
				}
#ifdef _DEBUG
				printf("�����������![%d]\n", dwRtn);
#endif
				if (dwRtn == ERROR_SERVICE_ALREADY_RUNNING) {
					dwRtn = 0;
					break;
				}
				if ((dwRtn == ERROR_IO_PENDING) && errNo < 5) {//������񴴽���Ҫ�ȴ�����ô�͵�15��
					errNo++;
					Sleep(3000);
					continue;
				}
				else {
					// _DebugPrint("�����������!", dwRtn);
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
		printf("ֹͣ�������![%d]\n", dwRtn);
#endif
		if (dwRtn == ERROR_IO_PENDING) {//������񴴽���Ҫ�ȴ�����ô�͵�15��
			errNo++;
			Sleep(3000);
			continue;
		}
		else {
			// _DebugPrint("ֹͣ�������!", dwRtn);
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
		printf("ɾ���������![%d]\n", dwRtn);
#endif
		if (dwRtn == ERROR_IO_PENDING) {//������񴴽���Ҫ�ȴ�����ô�͵�15��
			errNo++;
			Sleep(3000);
			continue;
		}
		else {
			// _DebugPrint("ɾ���������!", dwRtn);
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
			printf("�����ļ�����![%d]\n", dwRtn);
#endif
			if (dwRtn == ERROR_IO_PENDING && errNo < 5) {//������񴴽���Ҫ�ȴ�����ô�͵�15��
				errNo++;
				Sleep(3000);
				continue;
			}
			else {
				// _DebugPrint("�����ļ�����!", dwRtn);
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
	sprintf_s(szText, "���:%p �����:%d\n", this->handle, this->errCode);
	MessageBox(NULL, szText, "��ʾ", 64);
	*/
	while (this->errCode == 0 && !cR && errNo < 5) {
		cR = DeviceIoControl(this->handle,
			controlCode,  // ���ܺ�   
			inBuff,   // ���뻺�壬Ҫ���ݵ���Ϣ��Ԥ�����   
			inSize,  // ���뻺�峤��   
			outBuff,    // û���������   
			outSize,     // �������ĳ���Ϊ0   
			&rLength,   // ���صĳ���    
			NULL);
		dwRtn = GetLastError();
		if (dwRtn == 0) {
			break;
		}
#ifdef _DEBUG
		printf("���Ʒ������![%d]\n", dwRtn);
#endif
		if (dwRtn == ERROR_IO_PENDING) {
			errNo++;
			Sleep(3000);
			continue;
		}
		else {
			this->stepCode = 7;
			// _DebugPrint("���Ʒ������!", dwRtn);
			/*
			sprintf_s(szText, "���Ʒ������!:%d\n", dwRtn);
			MessageBox(NULL, szText, "��ʾ", 64);
			*/
			this->errCode = dwRtn;
			break;
		}
	}
	/*
	sprintf_s(szText, "���س���!:%d\n", rLength);
	MessageBox(NULL, szText, "��ʾ", 64);
	*/
	return cR ? rLength : 0;
}

void driver_loader::close() {
	if (this->device && this->device != INVALID_HANDLE_VALUE)
		CloseServiceHandle(this->device);
	this->device = NULL;
}