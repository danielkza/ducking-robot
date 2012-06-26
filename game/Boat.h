#ifndef Boat_H
#define Boat_H

#include <assert.h>

#include <SDL.h>

#include "Ent.h"
#include "VisibleEnt.h"

const ent_class_t Boat_CLASS;

#define Boat_STRUCT \
    VisibleEnt_STRUCT; \
    int image_index; \
    Uint32 last_rot_time

typedef struct Boat {
    Boat_STRUCT;
} Boat;

CLS_DEF_ACCESSOR(Boat, int, image_index);
CLS_DEF_ACCESSOR(Boat, Uint32, last_rot_time);

#define Boat_GET(member, obj) \
    CLS_DEF_GET(Boat, member, obj)

#define Boat_SET(member, obj, ...) \
    CLS_DEF_SET(Boat, member, obj, ##__VA_ARGS__)

#define Boat_CALL(method, obj, ...) \
    CLS_DEF_CALL(Boat, method, obj, ##__VA_ARGS__)


void Boat_m_init(Ent *ent);
void Boat_m_destroy(Ent *ent);
void Boat_m_spawn(Ent *ent);
void Boat_m_think(Ent *ent);

#endif