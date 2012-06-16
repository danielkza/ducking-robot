#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "list.h"
#include "Ent.h"

#include "events.h"

#ifdef _MSC_VER
#define strcasecmp stricmp
#endif

typedef struct {
	event_listener_func callback;
	void *param;
} event_listener_t;

static struct {
	char *type;
	list_t *listeners;
} event_table[EVENT_MAX];

static int event_table_count = -1;

int
events_init()
{
	if(event_table_count >= 0)
		return -1;

	event_table_count = 0;
	memset(event_table, 0, sizeof(event_table));

	return 0;
}

void
events_shutdown()
{
	int i;

	if(event_table_count < 0)
		return;

	for(i = 0; i < EVENT_MAX; i++)
	{
		if(event_table[i].type == NULL)
			continue;

		free(event_table[i].type);
		list_free(event_table[i].listeners);
	}
}

static unsigned int
events_hash_string(const char *string)
{
	return 0;
}

static int
events_find_pos(const char *type)
{
	unsigned int hash, pos;
	
	hash = events_hash_string(type) % EVENT_MAX;
	pos = hash;
	
	do
	{
		if(event_table[pos].type == NULL || strcasecmp(type, event_table[pos].type) == 0)
			return pos;

		pos++;
	} while(pos != hash);

	return -1;
}

event_handle_t
events_register(const char *type)
{
	int pos;

	if(type == NULL || type[0] == '\0')
		return -1;

	pos = events_find_pos(type);
	if(pos == -1)
		return -1;

	if(event_table[pos].type == NULL) {
		int len = strlen(type);
		char *type_copy = malloc(len + 1);
		assert(type_copy != NULL);

		memcpy(type_copy, type, len);
		type_copy[len] = '\0';

		event_table[pos].type = type_copy;
		event_table[pos].listeners = list_create();
	}

	return pos;
}

int
events_listen(event_handle_t event_handle,
              event_listener_func callback,
              void *param)
{
    event_listener_t *listener;

	if(event_handle < 0 || event_handle >= EVENT_MAX)
        return -1;

	listener = malloc(sizeof(*listener));
	assert(listener != NULL);

	listener->callback = callback;
	listener->param = param;

	list_push_front(event_table[event_handle].listeners, listener);

	return 0;
}

int
events_fire(event_handle_t event_handle,
		    Ent *origin,
			void *data)
{
    event_t event;
    list_t *iter;
    event_listener_t *listener;

	if(event_handle < 0 || event_handle >= EVENT_MAX)
        return -1;

    event.handle = event_handle;
    event.data = data;
    event.origin = origin;
    event.time = SDL_GetTicks();

    list_for_each_item(event_table[event_handle].listeners, iter, listener)
    {
        if((*listener->callback)(listener->param, &event) == EVENT_CONSUME)
            return 1;
    }

    return 0;
}