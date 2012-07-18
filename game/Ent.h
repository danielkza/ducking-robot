/**
  * @file
  * Ent base entity class
  **/


#ifndef Ent_H
#define Ent_H

#include <stddef.h>

#include <SDL.h>

#include "obj_defs.h"
#include "ent_base.h"
#include "vec2.h"
#include "game_time.h"

// Ent type definitions

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
    float bounds_width; \
    float bounds_height; \
    SDL_Rect bounds_rect; \
    \
    void (*m_spawn)(struct Ent*); \
    void (*m_remove)(struct Ent*); \
    void (*m_think)(struct Ent*); \
    \
    void (*m_touch)(struct Ent*, struct Ent*); \
    \
    void (*m_on_update)(struct Ent*); \
    void (*m_on_frame)(struct Ent *)

struct Ent {
    Ent_STRUCT;
};

enum {
    EFLAGS_NONE = 0,
    EFLAGS_VISIBLE   = 1<<0,
    EFLAGS_TOUCHABLE = 1<<1,
    EFLAGS_SOLID     = 1<<2,
    EFLAGS_ATTACHED  = 1<<3,
};

CLS_DEF_ACCESSOR(Ent, Ent *, parent);
CLS_DEF_ACCESSOR(Ent, Uint32, flags);
CLS_DEF_ACCESSOR_INDIRECT(Ent, vec2, position);
CLS_DEF_ACCESSOR_INDIRECT(Ent, vec2, move_direction);
CLS_DEF_ACCESSOR(Ent, float, speed);
CLS_DEF_ACCESSOR(Ent, float, max_speed);
CLS_DEF_ACCESSOR(Ent, float, rotation);
CLS_DEF_ACCESSOR(Ent, float, rot_speed);
CLS_DEF_ACCESSOR(Ent, float, max_rot_speed);
CLS_DEF_ACCESSOR(Ent, Uint32, prev_think);
CLS_DEF_ACCESSOR(Ent, Uint32, next_think);
CLS_DEF_ACCESSOR(Ent, float, bounds_width);
CLS_DEF_ACCESSOR(Ent, float, bounds_height);
CLS_DEF_ACCESSOR_INDIRECT(Ent, SDL_Rect, bounds_rect);

static inline Uint32
Ent_get_think_interval(Ent *ent)
{
    return game_time() - ent->prev_think;
}

static inline void
Ent_set_think_interval(Ent *ent, int interval)
{
    Uint32 cur_time = game_time();
    ent->prev_think = cur_time;
    ent->next_think = cur_time + interval;
}

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
    CLS_DEF_SET(Ent, member, obj, ##__VA_ARGS__)

#define Ent_CALL(method, obj, ...) \
    CLS_DEF_CALL(Ent, method, obj, ##__VA_ARGS__)


void Ent_m_init(Ent *ent);
void Ent_m_destroy(Ent *ent);
void Ent_m_spawn(Ent *ent);
void Ent_m_remove(Ent *ent);
void Ent_m_think(Ent *ent);
void Ent_m_touch(Ent *ent, Ent *ent2);
void Ent_m_on_update(Ent *ent);
void Ent_m_on_frame(Ent *ent);

void Ent_update(Ent *ent, Uint32 last_frame_time);

#endif
