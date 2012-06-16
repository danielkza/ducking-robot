#ifndef EVENTS_H
#define EVENTS_H

#include <limits.h>

#include <SDL.h>

#include "Ent.h"

#define EVENT_ERROR -1
#define EVENT_PASS 0
#define EVENT_CONSUME 1

#define EVENT_MAX 1024

typedef int event_handle_t;

typedef struct {
	event_handle_t handle;
	Uint32 time;
	Ent *origin;
	void *data;
} event_t;

typedef int (*event_listener_func)(void *param, const event_t *event);

int
events_init();

void
events_shutdown();

event_handle_t
events_register(const char *type);

int
events_listen(event_handle_t event_handle, 
              event_listener_func callback,
			  void *param);

int
events_cancel_listen(event_handle_t event_handle,
                     event_listener_func callback,
                     void *param);

int
events_fire(event_handle_t event_handle,
            Ent *origin,
			void *data);

#endif