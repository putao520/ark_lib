#include <Windows.h>
#include "process_adjust.h"

namespace GsProcess {
	void AdjustPrivilege(const char* SeName) {
		HANDLE hToken;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
			TOKEN_PRIVILEGES tp;
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (LookupPrivilegeValue(NULL, SeName, &tp.Privileges[0].Luid))
			{
				AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
			}
			CloseHandle(hToken);
		}
	}
	void AdjustPrivilege() {
		return AdjustPrivilege(SE_DEBUG_NAME);
	}
}