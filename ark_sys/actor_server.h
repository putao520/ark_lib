#pragma once
#include "../ark_actor/actor_common.h"

EXTERN_C_START

void* CallActorServiceGen(task_block_parameter* parameter, uintptr_t size, void* target);
actor_server* NewActorServer(HANDLE eHandle);
void DeleteActorServer(actor_server* self);

EXTERN_C_END