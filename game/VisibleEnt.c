#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#include "ent.h"
#include "VisibleEnt.h"

const ent_class_t VisibleEnt_CLASS = {
    "VisibleEnt", sizeof(VisibleEnt), &Ent_CLASS,
    VisibleEnt_m_init, VisibleEnt_m_destroy
};

void VisibleEnt_m_init(Ent *ent)
{
    VisibleEnt *v_ent = (VisibleEnt*)ent;
        
    v_ent->image = NULL;
    v_ent->visible = 0;

    v_ent->m_on_frame = VisibleEnt_m_on_frame;
    v_ent->m_draw = VisibleEnt_m_draw;
}

void VisibleEnt_m_destroy(Ent *ent)
{
    VisibleEnt *v_ent = (VisibleEnt*)ent;

    if(v_ent->image != NULL)
        SDL_FreeSurface(v_ent->image);
}

void VisibleEnt_m_on_frame(Ent *ent)
{
    VisibleEnt *v_ent = (VisibleEnt *)ent;

    VisibleEnt_CALL(draw, v_ent);
}

void VisibleEnt_m_draw(VisibleEnt *ent)
{
    const vec2 *position;
    SDL_Surface *image;

//    if(ent->image == NULL)
//        return;

    position = Ent_GET(position, ent);
    image = VisibleEnt_GET(image, ent);

    printf("Drawing VisibleEnt at position %d, %d with image %p\n",
           position->x, position->y, image);

    return;
}
