#include "ActorService.h"
#include "DriverLoaderService.h"

thread_local actor_client* current_actor = NULL;
actor_client* ActorService::current() {
	if (!current_actor)
		current_actor = actor_client::New(DriverLoaderService::current());

	return current_actor;
}

InternalServices* ActorService::services() {
	return current()->services();
}