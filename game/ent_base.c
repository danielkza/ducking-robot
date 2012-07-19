#include <stdlib.h>

#include "Ent.h"
#include "ent_table.h"

#include "ent_base.h"

int
ent_class_is_subclass(const ent_class_t *test,
                      const ent_class_t *base)
{
    for(; test != NULL; test = test->base) {
        if(test == base)
            return 1;
    }

    return 0;
}

static void
Ent_create_init(Ent *ent, const ent_class_t *eclass)
{
    if(eclass->base != NULL)
        Ent_create_init(ent, eclass->base);

    (*eclass->m_init)(ent);
}

Ent *
Ent_m_create(const ent_class_t *eclass)
{
    Ent *ent;
    
    if(eclass == NULL)
        return NULL;

    ent = malloc(eclass->data_size);
    if(ent == NULL)
        return NULL;

    ent->eclass = eclass;
    Ent_create_init(ent, eclass);
    ent_table_insert(ent);

    return ent;
}

void
Ent_destroy(Ent *ent, const ent_class_t *eclass)
{
    (*eclass->m_destroy)(ent);

    if(eclass->base != NULL)
        Ent_destroy(ent, eclass->base);
}

void
Ent_m_free(Ent *ent)
{
    if(ent != NULL) {
        ent_table_remove(ent);
        Ent_destroy(ent, ent->eclass);
        free(ent);
    }
}