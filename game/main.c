#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <SDL.h>
#include <SDL_main.h>
#include <SDL_ttf.h>

#include "assets.h"
#include "ent_table.h"
#include "pause.h"

#include "Boat.h"
#include "Enemy.h"

#define FRAME_RATE_MAX 60

SDL_Surface *pause_text;

void systems_init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    
    atexit(SDL_Quit);
    atexit(TTF_Quit);

    srand((unsigned int)time(NULL));

    assets_init();
    ent_table_init();

    set_started(0);
    set_game_over(0);
    set_paused(1);
}

void systems_shutdown()
{
    ent_table_shutdown();
    assets_shutdown();
}

void create_images(SDL_Surface **press_enter_text, SDL_Surface **pause_text, SDL_Surface **game_over_text, SDL_Surface **background)
{
    const asset_data_t *bg_asset;
    TTF_Font *font;
    SDL_Color white = {255, 255, 255, 255};

    bg_asset = assets_load(ASSET_TYPE_SURFACE, "images/water.tga");
    *background = bg_asset->surface;
    assert(*background != NULL);

    font = TTF_OpenFont("fonts/DroidSans-Bold.ttf", 24);
    assert(font != NULL);

    *press_enter_text = TTF_RenderText_Blended(font, "Press ENTER to start!", white);
    assert(*press_enter_text != NULL);
    
    *pause_text = TTF_RenderText_Blended(font, "PAUSE", white);
    assert(*pause_text != NULL);

    *game_over_text = TTF_RenderText_Blended(font, "GAME\nOVER", white);
    assert(*game_over_text != NULL);

    TTF_CloseFont(font);
}

void create_entities()
{
    vec2 position;
    const SDL_VideoInfo *video_info = SDL_GetVideoInfo();
 
    Boat *boat;
    Enemy *enemy;
    Ent *wall_top, *wall_bot, *wall_left, *wall_right;

    position.x = 0;
    position.y = 0;
    wall_top = ENT_CREATE(Ent);
    Ent_SET(flags, wall_top, EFLAGS_TOUCHABLE | EFLAGS_SOLID);
    Ent_SET(position, wall_top, &position);
    Ent_SET(bounds_width, wall_top, (float)video_info->current_w);
    Ent_SET(bounds_height, wall_top, 1);
    Ent_CALL(spawn, wall_top);
    
    position.x = 0;
    position.y = (float)(video_info->current_h - 1);
    wall_bot = ENT_CREATE(Ent);
    Ent_SET(flags, wall_bot, EFLAGS_TOUCHABLE | EFLAGS_SOLID);
    Ent_SET(position, wall_bot, &position);
    Ent_SET(bounds_width, wall_bot, (float)video_info->current_w);
    Ent_SET(bounds_height, wall_bot, 1);
    Ent_CALL(spawn, wall_bot);

    position.x = 0;
    position.y = 0;
    wall_left = ENT_CREATE(Ent);
    Ent_SET(flags, wall_left, EFLAGS_TOUCHABLE | EFLAGS_SOLID);
    Ent_SET(position, wall_left, &position);
    Ent_SET(bounds_width, wall_left, 1);
    Ent_SET(bounds_height, wall_left, (float)video_info->current_h);
    Ent_CALL(spawn, wall_left);

    position.x = (float)(video_info->current_w - 1);
    position.y = 0;
    wall_right = ENT_CREATE(Ent);
    Ent_SET(flags, wall_right, EFLAGS_TOUCHABLE | EFLAGS_SOLID);
    Ent_SET(position, wall_right, &position);
    Ent_SET(bounds_width,wall_right, 1);
    Ent_SET(bounds_height, wall_right, (float)video_info->current_h);
    Ent_CALL(spawn, wall_right);
    
    
    position.x = (float)(video_info->current_w / 2);
    position.y = (float)(video_info->current_h / 2);
    
    boat = ENT_CREATE(Boat);
    Ent_SET(position, boat, &position);
    Ent_CALL(spawn, boat);

    enemy = ENT_CREATE(Enemy);
    Ent_CALL(spawn, enemy);

    enemy = ENT_CREATE(Enemy);
    Ent_CALL(spawn, enemy);
    
    enemy = ENT_CREATE(Enemy);
    Ent_CALL(spawn, enemy);
    
    enemy = ENT_CREATE(Enemy);
    Ent_CALL(spawn, enemy);
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
            else if(event.key.keysym.sym == SDLK_RETURN) {
                if(is_paused()) {
                    if(!is_started()) {
                        set_started(1);
                    }
                    else if(is_game_over()) {
                        set_game_over(0);
                        ent_table_shutdown();
                        ent_table_init();
                        create_entities();
                    }
                }

                toggle_paused();
            }
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
draw_centered(SDL_Surface *screen, const SDL_VideoInfo *video_info, SDL_Surface *pause_text)
{
    SDL_Rect pos = {0, 0, 0, 0};

    pos.w = pause_text->w;
    pos.h = pause_text->h;
    pos.x = (video_info->current_w - pos.w) / 2;
    pos.y = (video_info->current_h - pos.h) / 2;

    SDL_BlitSurface(pause_text, NULL, screen, &pos);
}

void
update_entities(Uint32 last_frame_time)
{
    const list_t *ent_iter = NULL;

    /* Ent updating follows, in 3 stages:
     * -First stage: update position, angles and bounds for all entities
     * -Second stage: call the think functions on any entities necessary
     * -Third stage: call frame callbacks, which check collisions and draw to surface
     */

    if(!is_paused()) {    
        while((ent_iter = ent_table_next(ent_iter)) != NULL) {
            Ent *ent = ent_iter->item;
            if(game_time() >= Ent_GET(next_think, ent))
                Ent_CALL(think, ent);
        }
    
        while((ent_iter = ent_table_next(ent_iter)) != NULL) {
            Ent *ent = ent_iter->item;
            Ent_update(ent, last_frame_time);
        }
    }
    
    while((ent_iter = ent_table_next(ent_iter)) != NULL) {
        Ent *ent = ent_iter->item;
        Ent_CALL(on_frame, ent);
    }
}

void
start_frame(Uint32 *frame_start, Uint32 *frame_end)
{
    *frame_start = SDL_GetTicks();
    if(!is_paused())
        game_time_update(*frame_start - *frame_end);
}

void
finish_frame(Uint32 *frame_start, Uint32 *frame_end, Uint32 *game_frame_end)
{
    Uint32 delay = 1000 / FRAME_RATE_MAX, elapsed;

    *frame_end = SDL_GetTicks();
    elapsed = *frame_end - *frame_start;

    if(elapsed < delay)
        delay -= elapsed;

    *frame_end = SDL_GetTicks();
    if(!is_paused()) {
        game_time_update(elapsed);
        *game_frame_end = game_time();
    }
    
    SDL_Delay(delay);
}

int main(int argc, char **argv)
{
    SDL_Surface *screen, *background,
                *pause_text, *press_enter_text, *game_over_text;

    const SDL_VideoInfo *video_info;
    Uint32 frame_start, frame_end = 0, game_frame_end = 0;

    systems_init();

    screen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE|SDL_DOUBLEBUF);
    video_info = SDL_GetVideoInfo();
    
    create_images(&press_enter_text, &pause_text, &game_over_text, &background);
    create_entities();
 
    for(;;) {
        start_frame(&frame_start, &frame_end);

        if(check_SDL_events())
            break;

        draw_background(background, screen, video_info);

        update_entities(game_frame_end  );

        if(is_paused()) {
            if(is_game_over()) {
                draw_centered(screen, video_info, game_over_text);
            } else if(is_started()) {
                draw_centered(screen, video_info, pause_text);
            } else {
                draw_centered(screen, video_info, press_enter_text);
            }
        }

        SDL_Flip(screen);

        finish_frame(&frame_start, &frame_end, &game_frame_end);
    }

    SDL_FreeSurface(background);
    SDL_FreeSurface(pause_text);
    SDL_FreeSurface(press_enter_text);

    systems_shutdown();
    return 0;
}
