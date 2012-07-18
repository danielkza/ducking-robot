#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "utils.h"

void
SDL_DrawRect(SDL_Surface *dest,
             const SDL_Rect *rect,
             Uint32 color,
             int thickness)
{
    SDL_Rect cur_line;

    // Top
    cur_line.x = rect->x - (thickness / 2);
    cur_line.y = rect->y - (thickness / 2);
    cur_line.w = rect->w + thickness;
    cur_line.h = thickness;

    SDL_FillRect(dest, &cur_line, color);
    
    // Left
    cur_line.x = rect->x - (thickness / 2);
    cur_line.y = rect->y - (thickness / 2);
    cur_line.w = thickness;
    cur_line.h = rect->h + thickness;

    SDL_FillRect(dest, &cur_line, color);

    // Right
    cur_line.x = rect->x + rect->w - (thickness / 2);
    cur_line.y = rect->y - (thickness / 2);
    cur_line.w = thickness;
    cur_line.h = rect->h + thickness;

    SDL_FillRect(dest, &cur_line, color);

    // Bottom
    cur_line.x = rect->x - (thickness / 2);
    cur_line.y = rect->y + rect->h - (thickness / 2);
    cur_line.w = rect->w + thickness;
    cur_line.h = thickness;

    SDL_FillRect(dest, &cur_line, color);
}