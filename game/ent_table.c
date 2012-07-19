#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "Ent.h"

#include "ent_table.h"

static list_t *ent_list;
static int ent_count;

void
ent_table_init()
{
    ent_list = list_create();
    ent_count = 0;
}

void
ent_table_shutdown()
{
    list_t *iter;
    Ent *ent;

    list_for_each_item(ent_list, iter, ent) {
        // ENT_FREE() tries to remove entities from the table, which wouldn't
        // make sense when we are destroying the table!

        Ent_destroy(ent, ent->eclass);
        free(ent);
    }

    list_free(ent_list);
    ent_count = -1;
}

void
ent_table_insert(Ent *ent)
{
    list_t *iter;
    Ent *existing_ent;

    list_for_each_item(ent_list, iter, existing_ent) {
        if(existing_ent == ent)
            return;
    }

    list_push(ent_list, ent);
    ent_count++;
}

void
ent_table_remove(Ent *ent)
{
    list_t *iter, *next;

    list_for_each_node(ent_list, iter, next) {
        Ent *existing_ent = iter->item;
        if(existing_ent == ent) {
            list_remove(iter);
            ent_count--;
        }
    }
}

const list_t *
ent_table_next(const list_t *prev)
{
    if(prev == NULL) {
        if(ent_list == NULL)
            return NULL;

        return ent_list->next;
    }

    if(prev->next == ent_list)
        return NULL;

    return prev->next;
}

const list_t *
ent_table_next_by_class(const list_t *prev, 
                        const ent_class_t *eclass,
                        int find_subclasses)
{
    const list_t *iter;
    Ent *ent;

    if(eclass == NULL)
        return NULL;
    
    if(prev == NULL)
        iter = ent_list->next;
    else
        iter = prev->next;

    while(iter != ent_list) {
        ent = iter->item;

        if(!find_subclasses ? (ent->eclass == eclass) : ent_class_is_subclass(ent->eclass, eclass))
            return iter;
    
        iter = iter->next;
    }

    return NULL;
}

int
ent_table_count()
{
    return ent_count;
}