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

	// ��������״̬ 0:δ���أ�1���Ѽ���
	uint32_t loadStatus = 0;

	// �������
	SC_HANDLE device = NULL;
	// �ļ����
	DWORD errCode = 0;

	void SCM();
	SC_HANDLE openOrCreate();

	DWORD start();
	DWORD remove();
	DWORD stop();

	void close();
public:
	// ���캯��,����sys�ļ�uri
	driver_loader();
	driver_loader(const char* _uri);
	~driver_loader();
	driver_loader* InitDevName(const char* lpszDriverName);
	driver_loader* setUri(const char* _uri);
	driver_loader* wait();
	// ���������(��������ʱ��Ч),ɾ�������ļ�
	string getPath();
	string getDevName();
	DWORD getError();

	// �ȴ�����
	driver_loader* load();
	DWORD send(DWORD controlCode, char* inBuff, DWORD inSize, char* outBuff, DWORD outSize);
	driver_loader* unload();
};