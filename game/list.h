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

list_t *
list_insert(list_t *prev,
            void *item);

list_t *
list_push(list_t *list,
          void *item);

list_t *
list_push_front(list_t *list,
                void *item);

void *
list_remove(list_t *pos);

void *
list_pop(list_t *list);

void *
list_pop_front(list_t *list);

void
list_splice(list_t *start, list_t *end, list_t *dest);

void
list_swap(list_t *a, list_t *b);

int
list_empty(list_t *list);

void *
list_front(list_t *list);

void *
list_back(list_t *list);

list_t *
list_copy(list_t *list);

#define list_for_each_node(head, iter, next) \
    for(iter = head->next; \
        iter != NULL && iter != head && ((next = iter->next) || 1); \
        iter = next)

#define list_for_each_item(head, iter, data) \
    for(iter = head->next; \
        iter != NULL && iter != head && ((data = iter->item) || 1); \
        iter = iter->next)

#define list_for_each_node_rev(head, iter, prev) \
    for(iter = head->prev; \
        iter != NULL && iter != head && ((prev= iter->prev) || 1); \
        iter = prev)

#define list_for_each_item_rev(head, iter, data) \
    for(iter = head->prev; \
        iter != NULL && iter != head && ((data = iter->item) || 1); \
        iter = iter->prev)

#endif /* LIST_H */
