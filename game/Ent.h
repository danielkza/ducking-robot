#ifndef ENT_H
#define ENT_H

#include <stddef.h>

#include <SDL.h>

#include "obj_defs.h"
#include "vec2.h"


typedef struct Ent Ent;

typedef struct ent_class_t {
    const char *name;
    size_t data_size;
    const struct ent_class_t *base;
    void (*m_init)(Ent *ent);
    void (*m_destroy)(Ent *ent);
} ent_class_t;

Ent *Ent_m_create(const ent_class_t *eclass);
#define ENT_CREATE(eclass) ((eclass*)Ent_m_create(&eclass##_CLASS))
void Ent_m_free(Ent *ent);
#define ENT_FREE(ent) Ent_m_free((Ent*)(ent))


const ent_class_t Ent_CLASS;

#define Ent_STRUCT \
    const ent_class_t *eclass; \
    \
    struct Ent *parent; \
    \
    unsigned int flags; \
    \
    vec2 position; \
    vec2 move_direction; \
    float speed; \
    float max_speed; \
    \
    float rotation; \
    float rot_speed; \
    float max_rot_speed; \
    \
    Uint32 prev_think; \
    Uint32 next_think; \
    \
    void (*m_spawn) (struct Ent*); \
    void (*m_remove)(struct Ent*); \
    void (*m_think) (struct Ent*); \
    \
    void (*m_touch)  (struct Ent*, struct Ent*); \
    \
    void (*m_on_frame)(struct Ent *)

struct Ent {
    Ent_STRUCT;
};


CLS_DEF_ACCESSOR(Ent, Uint32, flags);
CLS_DEF_ACCESSOR_INDIRECT(Ent, vec2, position);
CLS_DEF_ACCESSOR_INDIRECT(Ent, vec2, move_direction);
CLS_DEF_ACCESSOR(Ent, float, speed);
CLS_DEF_ACCESSOR(Ent, float, max_speed);
CLS_DEF_ACCESSOR(Ent, float, rotation);

CLS_DEF_ACCESSOR(Ent, float, rot_speed);
CLS_DEF_ACCESSOR(Ent, float, max_rot_speed);

static inline void
Ent_set_velocity_vec(Ent *ent, vec2 *vel)
{
    float speed = vec2_len(vel);
    vec2 direction = *vel;

    vec2_norm(&direction);

    Ent_set_move_direction(ent, &direction);
    Ent_set_speed(ent, speed);
}


#define Ent_GET(member, obj) \
    CLS_DEF_GET(Ent, member, obj)

#define Ent_SET(member, obj, ...) \
    CLS_DEF_SET(Ent, member, obj, __VA_ARGS__)

#define Ent_CALL(method, obj, ...) \
    CLS_DEF_CALL(Ent, method, obj, __VA_ARGS__)


void Ent_m_init(Ent *ent);
void Ent_m_destroy(Ent *ent);

void Ent_m_spawn(Ent *ent);
void Ent_m_remove(Ent *ent);
void Ent_m_think(Ent *ent);
void Ent_m_touch(Ent *ent, Ent *ent2);
void Ent_m_on_frame(Ent *ent);

#endif
