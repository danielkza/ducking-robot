/**
 * @file
 * Linked list data structure
 **/

#ifndef LIST_H
#define LIST_H

/** Linked list datatype **/
typedef struct list_t {
    /** Stored item **/
    void *item;
    /** Previous node **/
    struct list_t *prev;
    /** Next node **/
    struct list_t *next;
} list_t;

/**
 * Creates a new list.
 *
 * @returns Pointer to newly allocated list, or NULL on failure.
 **/
list_t *
list_create();

/**
 * Frees an existing list and all its nodes.
 *
 * @param list Pointer to list head
 **/
void
list_free(list_t *list);

/**
 * Inserts a new node after an existing node in a list.
 *
 * @param list Pointer to previous node
 * @param item Item to insert
 *
 * @returns Pointer to inserted node, or NULL on failure
 **/

list_t *
list_insert(list_t *prev,
            void *item);

/**
 * Inserts a new node at the end of an existing list.
 *
 * @param list Pointer to list head
 * @param item Item to insert
 *
 * @returns Pointer to inserted node, or NULL on failure
 **/

list_t *
list_push(list_t *list,
          void *item);

/**
 * Inserts a new node at the start (after the head) of an existing list.
 *
 * @param list Pointer to list head
 * @param item Item to insert
 *
 * @returns Pointer to inserted node, or NULL on failure
 **/

list_t *
list_push_front(list_t *list,
                void *item);

/**
 * Removes a node from a list.
 *
 * @param list Pointer to node to remove
 *
 * @returns Item previously stored on node, or NULL on failure
 **/

void *
list_remove(list_t *pos);

/**
 * Removes the last node (excluding the head) from a list.
 *
 * @param list Pointer to list head
 *
 * @returns Item previously stored on node, or NULL on failure
 **/

void *
list_pop(list_t *list);


/**
 * Removes the first node after the head of a list.
 *
 * @param list Pointer to list head
 *
 * @returns Item previously stored on node, or NULL on failure
 **/

void *
list_pop_front(list_t *list);


/**
 * Moves a section of a list to a different place, possibly on a different list.
 * 
 * @param start Pointer to the first node to be moved
 * @param end   Pointer to _one item past_ the last node to be moved
 * @param dest  Pointer to the node that will preceded the moved section in the
 *              new order
 **/

void
list_splice(list_t *start, list_t *end, list_t *dest);

/**
 * Swaps items between two nodes, possibly in different lists.
 * 
 * @param a One node
 * @param b Another node
 **/

void
list_swap(list_t *a, list_t *b);

/**
 * Checks if a list is empty.
 *
 * @param list Pointer to list head
 *
 * @returns 1 if list is empty, 0 if it has any nodes other than the head
 **/

int
list_empty(list_t *list);

/**
 * Returns the item of the first node (after the head) on a list.
 *
 * @param list Pointer to list head
 *
 * @returns Item of the first list node, or NULL on failure
 **/

void *
list_front(list_t *list);

/**
 * Returns the item of the last node (excluding the head) on a list.
 *
 * @param list Pointer to list head
 *
 * @returns Item of the last list node, or NULL on failure
 **/

void *
list_back(list_t *list);

/**
 * Creates a copy of a list.
 * @param list Pointer to head of list to be copied
 *
 * @returns Pointer to head of a new list, with newly allocated nodes, with the
 *          same items from the original list, or NULL on failure
 **/

list_t *
list_copy(list_t *list);

/**
 * Loops through the nodes on a list.
 *
 * @param head Pointer to list head
 * @param iter Variable to store current node being iterated
 * @param next Variable to store the next node to be iterated
 *
 * Example of use:
 *
 * @code
 * 
 * list_t *my_list;
 * list_t *iter, *next;
 * list_for_each_node(my_list, iter, next) {
 *     printf("list item: %p\n", iter->item);
 * }
 *
 * @endcode
 **/

#define list_for_each_node(head, iter, next) \
    for(iter = head->next; \
        iter != NULL && iter != head && ((next = iter->next) || 1); \
        iter = next)

/**
 * Loops through the items contained in nodes on a list.
 *
 * @warning Does not pre-load next nodes to be iterated on. Changing the list
 *          mid-loop might cause unexpected behavior.
 *
 * @param head Pointer to list head
 * @param iter Variable to store current node being iterated
 * @param data Variable to store current item
 *
 * Example of use:
 *
 * @code
 * 
 * list_t *my_list;
 * list_t *iter;
 * void *data;
 * list_for_each_node(my_list, iter, data) {
 *     printf("list item: %p\n", data);
 * }
 *
 * @endcode
 **/

#define list_for_each_item(head, iter, data) \
    for(iter = head->next; \
        iter != NULL && iter != head && ((data = iter->item) || 1); \
        iter = iter->next)

/**
 * Loops through the nodes on a list _in reverse order_.
 *
 * @param head Pointer to list head
 * @param iter Variable to store current node being iterated
 * @param next Variable to store the next node to be iterated (previous on list
 *             order)
 *
 * Example of use:
 *
 * @code
 * 
 * list_t *my_list;
 * list_t *iter, *next;
 * list_for_each_node_rev(my_list, iter, next) {
 *     printf("list item: %p\n", iter->item);
 * }
 *
 * @endcode
 **/

#define list_for_each_node_rev(head, iter, prev) \
    for(iter = head->prev; \
        iter != NULL && iter != head && ((prev= iter->prev) || 1); \
        iter = prev)

/**
 * Loops through the items contained in nodes on a list _in reverse order_.
 *
 * @warning Does not pre-load next nodes to be iterated on. Changing the list
 *          mid-loop might cause unexpected behavior.
 *
 * @param head Pointer to list head
 * @param iter Variable to store current node being iterated
 * @param data Variable to store current item
 *
 * Example of use:
 *
 * @code
 * 
 * list_t *my_list;
 * list_t *iter;
 * void *data;
 * list_for_each_node_rev(my_list, iter, data) {
 *     printf("list item: %p\n", data);
 * }
 *
 * @endcode
 **/

#define list_for_each_item_rev(head, iter, data) \
    for(iter = head->prev; \
        iter != NULL && iter != head && ((data = iter->item) || 1); \
        iter = iter->prev)

#endif /* LIST_H */
