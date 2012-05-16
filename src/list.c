#include <stdlib.h>

#include "list.h"

list_t *
list_create()
{
    list_t *list = malloc(sizeof(*list));
    if(list == NULL)
        return NULL;

    list->prev = list->next = list;
    list->item = NULL;

    return list;
}

void
list_free(list_t *list)
{
    if(list != NULL) {
        while(list->prev != list)
            list_pop(list);

        free(list);
    }
}

void
list_push(list_t *list,
           void *item)
{
    if(list == NULL)
        return;

    list_t *entry = malloc(sizeof(*list));
    if(entry == NULL)
        return;

    entry->prev = list->prev;
    entry->next = list;
    list->prev->next = entry;
    list->prev = entry;

    entry->item = item;
}

void *
list_pop(list_t *list)
{
    if(list == NULL || list->prev == list)
        return NULL;

    list_t *entry = list->prev;
    
    list->prev = entry->prev;
    entry->prev->next = list;

    void *item = entry->item;
    free(entry);

    return item;
}

void *
list_front(list_t *list)
{
    if(list == NULL || list->next == list)
        return NULL;

    return list->next->item;
}


void *
list_tail(list_t *list)
{
    if(list == NULL || list->next == list)
        return NULL;

    return list->prev->item;
}

list_t *
list_copy(list_t *list)
{
    if(list == NULL)
        return NULL;

    list_t *new_list = list_create();
    if(new_list == NULL)
        return NULL;

    list_t *iter;
    void *item;

    list_for_each(list, iter, void*, item) {
        list_push(new_list, item);
    }

    return new_list;
}
