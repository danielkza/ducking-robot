#ifndef LIST_H
#define LIST_H

typedef struct list_t {
    void *item;
    struct list_t *prev, *next;
} list_t;

list_t *
list_create();

void
list_free(list_t *list);

void
list_push(list_t *list,
          void *item);

void *
list_pop(list_t *list);

void *
list_front(list_t *list);

void *
list_tail(list_t *list);

list_t *
list_copy(list_t *list);

#define list_for_each(head, iter, data) \
    for(iter = head->next; \
        iter != head && ((data = iter->item) || 1); \
        iter = iter->next)

#endif /* LIST_H */
