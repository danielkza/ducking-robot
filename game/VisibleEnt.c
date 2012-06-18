#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#include "assets.h"

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
    const asset_data_t *test_image = assets_load(ASSET_TYPE_SURFACE,
		                                         "images/boat.tga");

	v_ent->image = test_image->surface;
    
    rect->x = rect->y = 0;
    if(v_ent->image != NULL) {
        rect->w = v_ent->image->w;
        rect->h = v_ent->image->h;
    } else {
		printf("IMG_Load: %s\n", IMG_GetError());
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
	Uint8 *key_state = SDL_GetKeyState(NULL);
	vec2 move_direction = {0, 0};

	if(key_state[SDLK_LEFT]) {
		move_direction.x += -1;
	} else if(key_state[SDLK_RIGHT]) {
		move_direction.x += +1;
	}
	
	if(key_state[SDLK_UP]) {
		move_direction.y += -1;
	} else if(key_state[SDLK_DOWN]) {
		move_direction.y += +1;
	}

	vec2_norm(&move_direction);
	ent->move_direction = move_direction;
	ent->speed = 250;

	Ent_m_on_frame(ent);

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

    printf("Drawing VisibleEnt at position %f, %f with image %p\n",
           position->x, position->y, image);

    dest.x = (int)position->x;
    dest.y = (int)position->y;

    screen = SDL_GetVideoSurface();
    SDL_BlitSurface(image, &ent->image_rect, screen, &dest);
}
