#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_main.h>

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
    SDL_Surface *screen;
     VisibleEnt *v_ent; 
    Ent *ent;

    SDL_Init(SDL_INIT_EVERYTHING);
    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE|SDL_DOUBLEBUF);
    reset_stdout();

    ent = ENT_CREATE(Ent);
    v_ent = ENT_CREATE(VisibleEnt);

    Ent_CALL(spawn, ent);
    Ent_CALL(spawn, v_ent);

    for(;;) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                goto done;
        }

        SDL_Delay(1000 / 5);
        Ent_CALL(on_frame, ent);
        Ent_CALL(on_frame, v_ent);
    }

done:

    ENT_FREE(ent);
    ENT_FREE(v_ent);

    return 0;
}