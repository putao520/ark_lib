#pragma once

namespace memory_until {
	template<typename T>
	inline T ngx_align(T d, T a) {
		return ((d)+(a - 1)) & ~(a - 1);
	}
}