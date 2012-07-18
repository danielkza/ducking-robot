#ifndef UTILS_H
#define UTILS_H

#define _USE_MATH_DEFINES
#include <math.h>

#include <SDL.h>
#include <SDL_video.h>

#ifdef _MSC_VER
#define inline __inline
#endif

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#define clamp(a, a_min, a_max) \
    max(min((a), (a_max)), (a_min))

static inline float
roundf(float num)
{
    return (float)((int)((num >= 0) ? (num + 0.5) : (num - 0.5)));
}

static inline float
angle_normalize(float angle)
{
    if(angle < -180)
        angle += 360;
    else if(angle >= 180)
        angle -= 360;

    return angle;
}

static inline float
angle_round(float angle, int mod)
{
    float quot = angle / mod;
    return roundf(quot) * mod;
}

static inline float 
angle_to_rads(float degs)
{
    return (float)(degs / 180 * M_PI);
}

static inline float
angle_to_degs(float rads)
{
    return (float)(rads * 180 / M_PI);
}

void
SDL_DrawRect(SDL_Surface *dest,
             const SDL_Rect *rect,
             Uint32 color,
             int thickness);

#endif