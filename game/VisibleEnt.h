#ifndef VISIBLEENT_H
#define VISIBLEENT_H

#include <assert.h>

#include <SDL.h>

#include "ent.h"


const ent_class_t VisibleEnt_CLASS;

#define VisibleEnt_STRUCT \
    Ent_STRUCT; \
    \
    SDL_Surface *image; \
    int visible; \
    \
    void (*m_draw)(struct VisibleEnt *ent)

typedef struct VisibleEnt {
    VisibleEnt_STRUCT;
} VisibleEnt;


CLS_DEF_ACCESSOR(VisibleEnt, int, visible);

CLS_DEF_GETTER(VisibleEnt, SDL_Surface *, image);
static inline void
VisibleEnt_set_image(VisibleEnt *ent, SDL_Surface *image)
{
    assert(image != NULL);
    ent->image = image;
}


#define VisibleEnt_GET(member, obj) \
    CLS_DEF_GET(VisibleEnt, member, obj)

#define VisibleEnt_SET(member, obj, ...) \
    CLS_DEF_SET(VisibleEnt, member, obj, __VA_ARGS__)

#define VisibleEnt_CALL(method, obj, ...) \
    CLS_DEF_CALL(VisibleEnt, method, obj, __VA_ARGS__)


void VisibleEnt_m_init(Ent *ent);
void VisibleEnt_m_destroy(Ent *ent);

void VisibleEnt_m_on_frame(Ent *ent);

void VisibleEnt_m_draw(VisibleEnt *ent);

#endif