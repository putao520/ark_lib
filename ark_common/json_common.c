#include "json_common.h"

int InitJsonAllocater(JsonAllocater** v) {
	if (!v)
		return -1;
	JsonAllocater* p = *v;
	if (!p) {
		p = _malloc(sizeof(JsonAllocater));
		p->allocater = zzz_AllocatorNew();
		p->ref_cnt = 0;
		*v = p;
	}
	else {
		p->ref_cnt++;
	}
	return 1;
}

int DestoryJsonAllocater(JsonAllocater** v) {
	if (!v)
		return -1;
	JsonAllocater* p = *v;
	if (!p)
		return -2;
	else {
		p->ref_cnt--;
		if (!p->ref_cnt) {
			zzz_AllocatorRelease(p->allocater);
			_free(p);
		}
		*v = NULL;
	}
	return 1;
}

void UpdateJsonAllocater(JsonAllocater* p) {
	if (!p)
		return;
	zzz_AllocatorRelease(p->allocater);
	p->allocater = zzz_AllocatorNew();
}

zzz_Value* zzz_ValueCopyAuto(zzz_Value* dest, zzz_Value* src) {
	// zzz_Value* r;
	if (dest->A == src->A) {
		return zzz_ValueCopyFrom(dest, src) ? src : NULL;
	}
	else {
		struct zzz_Allocator* A = dest->A;
		zzz_Value* n = zzz_ValueNew(A);
		return zzz_ValueCopyFrom(n, src) ? n : NULL;
	}
}
