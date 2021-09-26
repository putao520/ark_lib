#pragma once
#include <ntddk.h>
#include "../ark_actor/ark_common.h"

#ifdef _WIN64

	#ifdef _DEBUG
		#pragma comment(lib, "../x64/Debug[Sys]/ark_common.lib")
	#else
		#pragma comment(lib, "../x64/Release[Sys]/ark_common.lib")
	#endif
#else

#ifdef _DEBUG
#pragma comment(lib, "../x86/Debug[Sys]/ark_common.lib")
#else
#pragma comment(lib, "../x86/Release[Sys]/ark_common.lib")
#endif
#endif