#pragma once
#include <Windows.h>
#include <string>

using namespace std;

class driver_loader {
private:
	// �ļ���ַ,�����http��ͷ��path,�����Զ�����
	string path;
	const char* driverName = NULL;
	// ������ͨѶ�Ƿ�ʹ�ü���ģʽ
	bool isSecTranport = false;
	// ��������״̬ 0:δ���أ�1�������У�2�������� 4������
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
	// ���캯��,����sys�ļ�uri
	driver_loader();
	driver_loader(const char* _uri);
	~driver_loader();
	driver_loader* InitDevName(const char* lpszDriverName);
	driver_loader* setUri(const char* _uri);
	// ���������(��������ʱ��Ч),ɾ�������ļ�
	void clearContext();
	driver_loader* load();
	string getPath();
	string getDevName();
	BOOL hasError();
	DWORD getError();
	DWORD getStep();
	DWORD send(DWORD controlCode, char* inBuff, DWORD inSize, char* outBuff, DWORD outSize);
	// �ȴ�����
	driver_loader* wait();
	driver_loader* unload();
};