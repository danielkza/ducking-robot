#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_main.h>

#include "assets.h"
#include "ent_table.h"

#include "Boat.h"

#define FRAME_RATE_MAX 60

void systems_init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    atexit(SDL_Quit);

    assets_init();
    ent_table_init();
}

void systems_shutdown()
{
    ent_table_shutdown();
    assets_shutdown();
}

void create_entities()
{
    vec2 position;
    Boat *boat, *boat2;
    Ent *wall_top, *wall_bot, *wall_left, *wall_right;

    position.x = 100;
    position.y = 100;
    wall_top = ENT_CREATE(Ent);
    Ent_SET(flags, wall_top, EFLAGS_TOUCHABLE | EFLAGS_SOLID);
    Ent_SET(position, wall_top, &position);
    Ent_SET(bounds_width, wall_top, 250);
    Ent_SET(bounds_height, wall_top, 1);
    Ent_CALL(spawn, wall_top);
    
    position.x = 100;
    position.y = 350;
    wall_bot = ENT_CREATE(Ent);
    Ent_SET(flags, wall_bot, EFLAGS_TOUCHABLE | EFLAGS_SOLID);
    Ent_SET(position, wall_bot, &position);
    Ent_SET(bounds_width, wall_bot, 250);
    Ent_SET(bounds_height, wall_bot, 1);
    Ent_CALL(spawn, wall_bot);

    position.x = 100;
    position.y = 100;
    wall_left = ENT_CREATE(Ent);
    Ent_SET(flags, wall_left, EFLAGS_TOUCHABLE | EFLAGS_SOLID);
    Ent_SET(position, wall_left, &position);
    Ent_SET(bounds_width, wall_left, 1);
    Ent_SET(bounds_height, wall_left, 250);
    Ent_CALL(spawn, wall_left);

    position.x = 350;
    position.y = 100;
    wall_right = ENT_CREATE(Ent);
    Ent_SET(flags, wall_right, EFLAGS_TOUCHABLE | EFLAGS_SOLID);
    Ent_SET(position, wall_right, &position);
    Ent_SET(bounds_width,wall_right, 1);
    Ent_SET(bounds_height, wall_right, 250);
    Ent_CALL(spawn, wall_right);

    boat = ENT_CREATE(Boat);
    Ent_CALL(spawn, boat);

    boat2 = ENT_CREATE(Boat);

    {
    vec2 pos = {250, 250};
    Ent_SET(position, boat2, &pos);
    }

    Ent_CALL(spawn, boat2);
    Ent_SET(max_speed, boat2, 0.1f);
}

int check_SDL_events()
{
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT)
            return 1;
        else if(event.type == SDL_KEYDOWN) {
            if(event.key.keysym.sym == SDLK_ESCAPE)
                return 1;
        }
    }

    return 0;
}


void
draw_background(SDL_Surface *background,
                SDL_Surface *screen,
                const SDL_VideoInfo *video_info)
{
    int col, row;
    for(col = 0; col < (video_info->current_w / 64) + 1; col++) {
        for(row = 0; row < (video_info->current_h / 64) + 1; row++) {
            SDL_Rect pos = {col * 64, row * 64, 0, 0};
            SDL_BlitSurface(background, NULL, screen, &pos);
        }
    }
}

void
update_entities(Uint32 last_frame_time)
{
    const list_t *ent_iter = NULL;

    /* Ent updating follows, in 3 stages:
        * -First stage: call the think functions on any entities necessary
        * -Second stage: update position, angles and bounds for all entities
        * -Third stage: call frame callbacks, which check collisions and draw to surface
        */
    while((ent_iter = ent_table_next(ent_iter)) != NULL) {
        Ent *ent = ent_iter->item;
        if(game_time() >= Ent_GET(next_think, ent))
            Ent_CALL(think, ent);
    }

    while((ent_iter = ent_table_next(ent_iter)) != NULL) {
        Ent *ent = ent_iter->item;
        Ent_update_pre_frame(ent, last_frame_time);
    }

    Ent_check_all_collisions();

    while((ent_iter = ent_table_next(ent_iter)) != NULL) {
        Ent *ent = ent_iter->item;
        Ent_CALL(on_frame, ent, last_frame_time);
    }
}

void
start_frame(Uint32 *frame_start, Uint32 *frame_end)
{
    *frame_start = SDL_GetTicks();
    game_time_update(*frame_start - *frame_end);
}

void
finish_frame(Uint32 *frame_start, Uint32 *frame_end)
{
    Uint32 delay = 1000 / FRAME_RATE_MAX, elapsed;

    *frame_end = SDL_GetTicks();
    elapsed = *frame_end - *frame_start;

    if(elapsed < delay)
        delay -= elapsed;

    *frame_end = SDL_GetTicks();
    game_time_update(elapsed);
    SDL_Delay(delay);
}

int main(int argc, char **argv)
{
    SDL_Surface *screen, *background;
    const SDL_VideoInfo *video_info;
    const asset_data_t *bg_asset;
    Uint32 frame_start, frame_end = 0;

    systems_init();

    screen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE|SDL_DOUBLEBUF);
    video_info = SDL_GetVideoInfo();

    bg_asset = assets_load(ASSET_TYPE_SURFACE, "images/water.tga");
    background = bg_asset->surface;
    
    create_entities();
 
    for(;;) {
        start_frame(&frame_start, &frame_end);

        if(check_SDL_events())
            break;

        draw_background(background, screen, video_info);

        update_entities(frame_end);

        SDL_Flip(screen);

        finish_frame(&frame_start, &frame_end);
    }

done:
    systems_shutdown();
    return 0;
}
