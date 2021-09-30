#include "ActorService.h"
#include "DriverLoaderService.h"
#include "debug_until.h"

thread_local actor_client* thread_actor = NULL;
actor_client* ActorService::current() {
	if (!thread_actor) {
		thread_actor = actor_client::New(DriverLoaderService::current())->connect();
	}
	return thread_actor;
}

InternalServices* ActorService::services() {
	return current()->services();
}

