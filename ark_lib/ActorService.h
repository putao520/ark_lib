#pragma once
#include "actor_client.h"

class ActorService {
public:
	static actor_client* current();
	static InternalServices* services();
};