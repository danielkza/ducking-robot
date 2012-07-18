#ifndef ENT_BASE_H
#define ENT_BASE_H

typedef struct Ent Ent;

typedef struct ent_class_t {
    const char *name;
    size_t data_size;
    const struct ent_class_t *base;
    void (*m_init)(Ent *ent);
    void (*m_destroy)(Ent *ent);
} ent_class_t;

int
ent_class_is_subclass(const ent_class_t *test, const ent_class_t *base);

Ent
*Ent_m_create(const ent_class_t *eclass);
#define ENT_CREATE(eclass) \
    ((eclass*)Ent_m_create(&eclass##_CLASS))

void
Ent_m_free(Ent *ent);
#define ENT_FREE(ent) \
    Ent_m_free((Ent*)(ent))

#endif