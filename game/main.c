#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_main.h>

#include "assets.h"

#include "Ent.h"
#include "VisibleEnt.h"

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
    VisibleEnt *v_ent; 
    Ent *ent;
	vec2 velocity = {250, 250};
	const asset_data_t *bg_asset;

    SDL_Init(SDL_INIT_EVERYTHING);
    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(0, 0, 0, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
    reset_stdout();

	bg_asset = assets_load(ASSET_TYPE_SURFACE, "images/water.tga");
	background = bg_asset->surface;

    ent = ENT_CREATE(Ent);
    v_ent = ENT_CREATE(VisibleEnt);

    Ent_CALL(spawn, ent);
    Ent_CALL(spawn, v_ent);

	Ent_set_velocity_vec((Ent*)v_ent, &velocity);

	video_info = SDL_GetVideoInfo();

    for(;;) {
		int row, col;
		int width, height;


		SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                goto done;
        }

        SDL_Delay(1000 / 1000);
        
		for(col = 0; col < (video_info->current_w / 64) + 1; col++) {
			for(row = 0; row < (video_info->current_h / 64) + 1; row++) {
				SDL_Rect pos = {col * 64, row * 64, 0, 0};
				SDL_BlitSurface(background, NULL, screen, &pos);
			}
		}

		Ent_CALL(think, ent);
		Ent_CALL(think, v_ent);

		Ent_CALL(on_frame, ent);
        Ent_CALL(on_frame, v_ent);

		SDL_Flip(screen);
    }

done:

    ENT_FREE(ent);
    ENT_FREE(v_ent);

    return 0;
}