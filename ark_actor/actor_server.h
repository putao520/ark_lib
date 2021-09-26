#pragma once
#include "actor_common.h"
#include "../ark_common/gs_array.h"

EXTERN_C_START

void* CallActorServiceGen(uintptr_t* parameter, uintptr_t size, void* target);
actor_server* NewActorServer(HANDLE eHandle);
void DeleteActorServer(actor_server* self);
EXTERN_C_END