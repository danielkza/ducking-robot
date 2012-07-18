/**
  * @file
  * Event system
  **/


#ifndef EVENTS_H
#define EVENTS_H

#include <limits.h>

#include <SDL.h>

#include "Ent.h"

/** Event callback return values **/
enum {
    /** An error has occurred **/
    EVENT_ERROR   = -1,
    /** Continue propagating event / event was not consumed by any listeners **/
    EVENT_PASS    =  0,
    /** Stop propagating event / event was consumed by a listener **/
    EVENT_CONSUME =  1
};

/** Maximum number of event types (NOT events in flight) **/
#define EVENT_MAX 1024

/** Abstract handle for a registered event type **/
typedef int event_handle_t;

/** Event type **/
typedef struct {
    /** Event type handle **/
    event_handle_t handle;
    /** Generation time **/
    Uint32 time;
    /** Generating entity **/
    Ent *origin;
    /** Abstract data **/
    void *data;
} event_t;

/**
 * Event listening callback function type
 *
 * @param param User-defined data for internal use by callback handling.
 * @param event Pointer to event data
 *
 * @returns EVENT_ERROR, EVENT_PASS or EVENT_CONSUME
 **/
typedef int (*event_listener_func)(void *param, const event_t *event);

/**
 * Initializes internal data for the event system. Must be called before using
 * any other functions.
 */
int
events_init();


/**
 * Invalidates all internal state, and anfrees up all event data.
 * Existing event handles are invalidated. If any event is currently in-flight,
 * behavior is undefined.
 *
 * @warning does not do reference counting. Calling will immediately invalide all resources.
 **/
void
events_shutdown();

/**
 * Registers an event for use (firing and listening)
 *
 * @param type String used for idenetifying this event type.
 *
 * @returns Event handle, or a negative value on error.
 **/
event_handle_t
events_register(const char *type);

/**
 * Register a callback to listen to a previously-registered event type.
 *
 * Functions may be registed multiple times, possibly with different
 * user-defined parameters for different instances of data.
 * Each call to this function should be paired with an equivalent
 * events_cancel_listen() call
 *
 * @param handle   Existing event handle
 * @param callback Callback function
 * @param param    User-defined data, will be propagated to callback
 *
 * @returns 0 on success, non-0 on failure
 **/
int
events_listen(event_handle_t event_handle, 
              event_listener_func callback,
              void *param);

/**
 * Cancels a previously registered event callback function.
 *
 * @param handle   Existing event handle
 * @param callback Callback function
 * @param param    User-defined data, will be propagated to callback
 *
 * @returns 0 on success, non-0 on failure
 **/

int
events_cancel_listen(event_handle_t event_handle,
                     event_listener_func callback,
                     void *param);

/**
 * Fires an event, notifying registered listeners.
 *
 * Listeners are invoked in inverse registration order, which can be important
 * if any of them consumes the event, stopping further propagation.
 *
 * @param event_handle Event handle
 * @param origin       Entity responsible for triggering this event.
 *                     NULL if not applicable.
 * @param data         User-defined event data.
 *
 * @returns EVENT_ERROR, EVENT_PASS or EVENT_CONSUME
 **/

int
events_fire(event_handle_t event_handle,
            Ent *origin,
            void *data);

#endif