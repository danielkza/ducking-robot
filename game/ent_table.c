#include <stdlib.h>
#include <stdio.h>

#include "Ent.h"
#include "hash_fnv1a.h"
#include "list.h"

#include "ent_table.h"

typedef struct {
    const ent_class_t* eclass;
    int ent_count;
    list_t *ent_list;
    int ent_free_count;
    list_t *ent_free_list;
} ent_table_entry_t;

static ent_table_entry_t ent_table[ENT_TABLE_CLASS_MAX];

static int ent_table_class_count;
static int ent_table_count;

int
ent_table_init()
{
    ent_table_class_count = 0;
    ent_table_count = 0;
}

static void
ent_table_entry_free(ent_table_entry_t *entry)
{
    list_t *iter;
    Ent *ent;
    
    if(entry != NULL) {
        if(entry->ent_list != NULL) {
            list_for_each_item(entry->ent_list, iter, ent)
                ENT_FREE(ent);
        }

        if(entry->ent_free_list != NULL) {
            list_for_each_item(entry->ent_free_list, iter, ent)
                ENT_FREE(ent);
        }
    }
}

void
ent_table_shutdown()
{
    int i;
    for(i = 0; i < ent_table_class_count; i++)
        ent_table_entry_free(&ent_table[i]);

    ent_table_class_count = -1;
    ent_table_count = -1;
}

static unsigned int
ent_table_hash_class(const ent_class_t *eclass)
{
    hash_fnv1a(&eclass, sizeof(eclass))




'