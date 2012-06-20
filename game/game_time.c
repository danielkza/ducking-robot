#include <stdlib.h>

#include <SDL.h>

#include "game_time.h"

static Uint32 cur_time = 0;

Uint32
game_time()
{
    return cur_time;
}

Uint32
game_time_update(Uint32 ticks)
{
    cur_time += ticks;
}