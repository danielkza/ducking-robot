/**
  * @file
  * Central entity registration system
  **/

#ifndef ENT_TABLE_H
#define ENT_TABLE_H

#include "list.h"
#include "Ent.h"

/**
 * Initializes the entity table.
 * Must be called before using any other function.
 **/
void
ent_table_init();

/**
 * Destroys the entity table, __freeing all registered entities__.
 * All existing entity references __are invalidated__.
 * All existing entity iterators __are invalidated__.
 *
 * @warning Does not do reference counting. All data is immediately freed.
 **/
void
ent_table_shutdown();

/**
 * Inserts an entity into the table.
 * No provision for avoiding duplicate insertion is provided.
 *
 * @param ent Entity to register
 **/
void
ent_table_insert(Ent *ent);

/**
 * Removes a previously-registered entity from the table.
 *
 * @warning This function is automatically called from Ent destructors.
 *
 * @param ent Entity to remove.
 **/
void
ent_table_remove(Ent *ent);

/**
 * Returns an iterator to the first or next entity on the table.
 *
 * @param prev Previous entity iterated upon. If NULL, the function will 
 *             retrieve the first entity.
 *
 * @returns Next entity iterator, or NULL if no more are available
 *
 * Example of use:
 * @code

 * const list_t *iter = NULL;
 * while((iter = ent_table_next(iter)) != NULL) {
 *     Ent *ent = iter->item;
 *     printf("ent class: %s\n", ent->eclass->name);
 * }
 *
 * @endcode
 **/
const list_t *
ent_table_next(const list_t *prev);


/**
 * Returns an iterator to the first or next entity on the table which matches
 * a particular class type.
 *
 * @param prev            Previous entity iterated upon. If NULL, the function will 
 *                        retrieve the first entity.
 * @param eclass          Entity class to retrieve.
 * @param find_subclasses If 0, only exact matches of eclass will be accepted.
 *                        Otherwise, any entities which are objects of
 *                        subclasses of eclass will also be retrieved.
 *
 * @returns Next entity iterator, or NULL if no more are available
 *
 * Example of use:
 * @code

 * const list_t *iter = NULL;
 * while((iter = ent_table_next_by_class(iter, &Boat_CLASS, 1)) != NULL) {
 *     Boat *boat = iter->item;
 *     printf("%s is a Boat!\n", boat->eclass->name);
 * }
 *
 * @endcode
 **/

const list_t *
ent_table_next_by_class(const list_t *prev, 
                        const ent_class_t *eclass,
                        int find_subclasses);

/**
 * Retrieves the current count of registed entities on the table.
 *
 * @returns Count, or -1 if table is not initialized
 **/

int
ent_table_count();

#endif