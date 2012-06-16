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

list_t *
list_insert(list_t *prev,
            void *item)
{
    list_t *entry;

    if(prev == NULL)
        return NULL;

    entry = malloc(sizeof(*entry));
    if(entry == NULL)
        return NULL;

    entry->prev = prev;
    entry->next = prev->next;

    entry->next->prev = entry;
    entry->prev->next = entry;
    
    entry->item = item;
    return entry;
}

list_t *
list_push(list_t *list,
          void *item)
{
    return list == NULL ? NULL : list_insert(list->prev, item);
}

list_t *
list_push_front(list_t *list,
                void *item)
{
    return list == NULL ? NULL : list_insert(list, item);
}

void *
list_remove(list_t *pos)
{
    list_t *prev, *next;
    void *item;

    if(pos == NULL || pos->next == pos)
        return NULL;

    prev = pos->prev;
    next = pos->next;

    prev->next = next;
    next->prev = prev;

    item = pos->item;
    free(pos);

    return item;
}

void *
list_pop(list_t *list)
{
    return list_empty(list) ? NULL : list_remove(list->prev);
}

void *
list_pop_front(list_t *list)
{
    return list_empty(list) ? NULL : list_remove(list->next);
}

void
list_splice(list_t *start, list_t *end, list_t *dest)
{
    list_t *start_prev, *end_prev, *dest_next;
    if(start == NULL || end == NULL || dest == NULL || start == end)
        return;

    start_prev = start->prev,
    end_prev = end->prev,
    dest_next = dest->next;

    start_prev->next = end;
    end->prev = start_prev;

    start->prev = dest;
    dest->next = start;

    dest_next->prev = end_prev;
    end_prev->next = dest_next;
}

void
list_swap(list_t *a, list_t *b)
{
    void *a_item;
    if(a == NULL || b == NULL)
        return;

    a_item = a->item;
    a->item = b->item;
    b->item = a_item;
}

int
list_empty(list_t *list)
{
    return (list == NULL || list->next == list);
}

void *
list_front(list_t *list)
{
    return list_empty(list) ? NULL : list->next->item;
}

void *
list_back(list_t *list)
{
    return list_empty(list) ? NULL : list->prev->item;
}

list_t *
list_copy(list_t *list)
{
    list_t *new_list, *iter;
    void *item;

    if(list == NULL)
        return NULL;

    new_list = list_create();
    if(new_list == NULL)
        return NULL;

    list_for_each_item(list, iter, item) {
        list_push(new_list, item);
    }

    return new_list;
}
