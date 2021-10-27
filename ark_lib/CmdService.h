#pragma once
#include "v8task_dispatch.h"

/**
* 负责与服务器通讯
*	接收任务投递到执行队列
*	发送返回队列数据到服务器
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