#include "library_manager.h"
#include "gs_main.h"
namespace gs_main {
	void checklibrary() {
		library_manager l;
		l.update();
	}

	void main() {
		// 判断更新

		// 检测库信息
		checklibrary();
		// 连接服务端

	}
}