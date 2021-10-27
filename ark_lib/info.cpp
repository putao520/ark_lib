#include "info.h"

namespace ark {
	namespace system {
		UINT32 GetCpuCoreTotalCount() {
			SYSTEM_INFO sysInfo;
			GetSystemInfo(&sysInfo);
			return sysInfo.dwNumberOfProcessors;
		}
	}
}