#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#include "assets.h"
#include "utils.h"

#include "Ent.h"
#include "VisibleEnt.h"

const ent_class_t VisibleEnt_CLASS = {
    "VisibleEnt", sizeof(VisibleEnt), &Ent_CLASS,
    VisibleEnt_m_init, VisibleEnt_m_destroy
};

void VisibleEnt_m_init(Ent *ent)
{
    VisibleEnt *v_ent = (VisibleEnt*)ent;
    SDL_Rect *rect = &v_ent->image_rect;

    v_ent->image = NULL;
    
    v_ent->m_on_frame = VisibleEnt_m_on_frame;
    v_ent->m_draw = VisibleEnt_m_draw;
}

void VisibleEnt_m_destroy(Ent *ent)
{
    VisibleEnt *v_ent = (VisibleEnt*)ent;

    if(v_ent->image != NULL)
        SDL_FreeSurface(v_ent->image);
}

void VisibleEnt_m_on_frame(Ent *ent, Uint32 frame_time)
{
    Ent_m_on_frame(ent, frame_time);
    VisibleEnt_CALL(draw, ent);
}

void VisibleEnt_m_draw(VisibleEnt *ent)
{
    const vec2 *position;
    SDL_Surface *image, *screen;
    SDL_Rect dest = {0, 0, 0, 0};

    if(!(Ent_GET(flags, ent) & EFLAGS_VISIBLE) || (image = VisibleEnt_GET(image, ent)) == NULL)
        return;

    position = Ent_GET(position, ent);

    dest.x = (int)position->x;
    dest.y = (int)position->y;

    screen = SDL_GetVideoSurface();
    SDL_BlitSurface(image, &ent->image_rect, screen, &dest);
    SDL_DrawRect(screen, Ent_GET(bounds_rect, ent), 0xFFFFFFFF, 1);
}

static void
abs_rect_to_ent_surface_rect(VisibleEnt *ent, const SDL_Rect *src, SDL_Rect *dest)
{
    SDL_Rect result = *src;
    const SDL_Rect *image_rect = VisibleEnt_GET(image_rect, ent);
    const vec2 *position = Ent_GET(position, ent);
    
    // Make rectangle relative to ent position
    result.x -= position->x;
    result.y -= position->y;

    // Then convert relative positions to absolute positions inside the surface
    result.x += image_rect->x;
    result.y += image_rect->y;
    result.w = min(image_rect->w, result.w);
    result.h = min(image_rect->h, result.h);

    *dest = result;
}

static inline int
SDL_IsPixelTransparent(const SDL_Surface *surface, int x, int y)
{
    Uint32 pixel = ((Uint32*)(surface->pixels))[y * surface->w + x];
    return (pixel & surface->format->Amask) == 0;
}

int
VisibleEnt_check_single_collision(VisibleEnt *ent1,
                                  Ent *ent2,
                                  const SDL_Rect *intersection)
{
    const SDL_Surface *surf1 = NULL, *surf2 = NULL;
    SDL_Rect rect1, rect2;

    // Retrieve surface and it's drawing rectangle for the first entity.
    surf1 = VisibleEnt_GET(image, ent1);
    abs_rect_to_ent_surface_rect(ent1, intersection, &rect1);

    // Do the same for the second entity if it is a VisibleEnt.
    if(ent_class_is_subclass(ent2->eclass, &VisibleEnt_CLASS)) {
        surf2 = VisibleEnt_GET(image, ent2);
        abs_rect_to_ent_surface_rect((VisibleEnt*)ent2, intersection, &rect2);
    }
    
    if(surf1 == NULL && surf2 == NULL) {
        // No images available for either entity, which means nothing is visible,
        // so nothing collides.

        return 0;
    } else if(surf1 != NULL && surf2 != NULL) {
        // Both images are available, compare their corresponding pixels and find
        // out if there is any point where they are _both_ non-transparent.
        
        int min_w = min(rect1.w, rect2.w);
        int min_h = min(rect1.h, rect2.h);

        int y, x;
        for(y = 0; y < min_h; y++) {
            for(x = 0; x < min_w; x++) {
                if(!SDL_IsPixelTransparent(surf1, rect1.x + x, rect1.y + y)
                   && !SDL_IsPixelTransparent(surf2, rect2.x + x, rect2.y + y))
                {
                    return 1;
                }
            }
        }
    } else {
        // Only one image available, so we can simply see if there is any
        // non-transparent pixels in the collision region.
    
        const SDL_Surface *surf;
        const SDL_Rect *rect;
        int y, x;

        if(surf1 != NULL) {
            surf = surf1;
            rect = &rect1;
        } else {
            surf = surf2;
            rect = &rect2;
        }

        for(y = 0; y < rect->h; y++) {
            for(x = 0; x < rect->w; x++) {
                if(!SDL_IsPixelTransparent(surf, rect->x + x, rect->y + y))
                    return 1;
            }
        }
    }
        
    return 0;
}