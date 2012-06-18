#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#include "ent.h"
#include "VisibleEnt.h"

const ent_class_t VisibleEnt_CLASS = {
    "VisibleEnt", sizeof(VisibleEnt), &Ent_CLASS,
    VisibleEnt_m_init, VisibleEnt_m_destroy
};

void VisibleEnt_m_init(Ent *ent)
{
    VisibleEnt *v_ent = (VisibleEnt*)ent;
    SDL_Rect *rect = &v_ent->image_rect;
        
    v_ent->image = IMG_Load("./images/test.png");
    
    rect->x = rect->y = 0;
    if(v_ent->image != NULL) {
        rect->w = v_ent->image->w;
        rect->h = v_ent->image->h;
    } else {
        rect->w = rect->h = 0;
    }

    v_ent->visible = 1;

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
    SDL_Surface *image, *screen;
    SDL_Rect dest = {0, 0, 0, 0};

    if(!ent->visible || (image = ent->image) == NULL)
        return;

    position = Ent_GET(position, ent);

    printf("Drawing VisibleEnt at position %d, %d with image %p\n",
           position->x, position->y, image);

    dest.x = position->x;
    dest.y = position->y;

    screen = SDL_GetVideoSurface();
    SDL_BlitSurface(image, &ent->image_rect, screen, &dest);
}
