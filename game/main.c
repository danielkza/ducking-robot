#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_main.h>

#include "assets.h"

#include "Boat.h"

void reset_stdout()
{    
#ifdef WIN32
    FILE * ctt = fopen("CON", "w" );
    freopen( "CON", "w", stdout );
    freopen( "CON", "w", stderr );
#endif
}

int main(int argc, char **argv)
{
    SDL_Surface *screen, *background;
    const SDL_VideoInfo *video_info;
    Boat *boat, *boat2;
    const asset_data_t *bg_asset;
    Uint32 frame_start, frame_end = 0;

    SDL_Init(SDL_INIT_EVERYTHING);
    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE|SDL_DOUBLEBUF);
    reset_stdout();

    bg_asset = assets_load(ASSET_TYPE_SURFACE, "images/water.tga");
    background = bg_asset->surface;

    boat = ENT_CREATE(Boat);
    boat2 = ENT_CREATE(Boat);
    Ent_CALL(spawn, boat);
    Ent_CALL(spawn, boat2);

    {
    vec2 pos = {250, 250};
    Ent_SET(position, boat2, &pos);
    }

    video_info = SDL_GetVideoInfo();

    for(;;) {
        frame_start = SDL_GetTicks();
        game_time_update(frame_start - frame_end);
        
        {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                goto done;
            else if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    goto done;
            }
        }
        }

        SDL_Delay(1000 / 1000);
        
        {
        int col, row;
        for(col = 0; col < (video_info->current_w / 64) + 1; col++) {
            for(row = 0; row < (video_info->current_h / 64) + 1; row++) {
                SDL_Rect pos = {col * 64, row * 64, 0, 0};
                SDL_BlitSurface(background, NULL, screen, &pos);
            }
        }
        }

        if(game_time() >= Ent_GET(next_think, boat))
            Ent_CALL(think, boat);

        if(game_time() >= Ent_GET(next_think, boat2))
            Ent_CALL(think, boat2);    


        Ent_CALL(on_frame, boat);
        Ent_CALL(on_frame, boat2);

        SDL_Flip(screen);

        frame_end = SDL_GetTicks();
        game_time_update(frame_end - frame_start);
    }

done:
    ENT_FREE(boat);
    ENT_FREE(boat2);

    return 0;
}
