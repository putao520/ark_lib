#pragma once
#include "v8task_dispatch.h"

/**
* �����������ͨѶ
*	��������Ͷ�ݵ�ִ�ж���
*	���ͷ��ض������ݵ�������
* 
*/

class CmdService {
public:
	CmdService();
	~CmdService();
	void start();
	void stop();
private:
	v8TaskDispatch dispatch;
};