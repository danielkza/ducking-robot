/**
 * @file
 * Miscellaneous utilities
 **/

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

/**
 * Returns the smaller of two values.
 *
 * @warning Parameters should not have side effects as they will possibly be
 *          executed multiple times
 *
 * @param a A numeric value
 * @param b Another numeric value
 * 

 * @returns The smaller value.
 **/

#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max

/**
 * Returns the larger of two values.
 *
 * @warning Parameters should not have side effects as they will possibly be
 *          executed multiple times
 *
 * @param a A numeric value
 * @param b Another numeric value
 * 

 * @returns The larger value.
 **/

#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

/**
 * Restricts a value to an interval.
 *
 * @warning Parameters should not have side effects as they will possibly be
 *          executed multiple times
 *
 * @param a Value to check
 * @param a_min Left end of the interval
 * @param a_max Right end of the interval
 *
 * @returns Original value if it fits the interval, one of the ends if it does not.
 **/

#define clamp(a, a_min, a_max) \
    max(min((a), (a_max)), (a_min))

/**
 * Rounds a float to the nearest integer.
 *
 * @param num Number to round
 *
 * @returns Number rounded upwards or downwards, whichever is closer.
 **/

static inline float
round_float(float num)
{
    return (float)((int)((num >= 0) ? (num + 0.5) : (num - 0.5)));
}

/**
 * Normalizes an angle, translating it to the -180 -> +180 interval if needed.
 *
 * @param angle Angle to normalize
 *
 * @returns Equivalent angle in the -180 -> +180 interval.
 **/

static inline float
angle_normalize(float angle)
{
    while(angle < -180)
        angle += 360;
    while(angle >= 180)
        angle -= 360;

    return angle;
}

/**
 * Rounds an angle to the nearest multiple of a specified amount.
 *
 * @param angle Angle to be rounded.
 * @param mod   Divisor for the rounding: the result will be a multiple of it.
 *
 * @returns Rounded angle.
 **/

static inline float
angle_round(float angle, int mod)
{
    float quot = angle / mod;
    return round_float(quot) * mod;
}

/**
 * Converts an angle from degrees to radians.
 *
 * @param degs Angle in degrees.
 *
 * @returns Angle in radians.
 **/

static inline float 
angle_to_rads(float degs)
{
    return (float)(degs / 180 * M_PI);
}

/**
 * Converts an angle from radians to degrees.
 *
 * @param rads Angle in radians
 *
 * @returns Angle in degrees.
 **/

static inline float
angle_to_degs(float rads)
{
    return (float)(rads * 180 / M_PI);
}

/**
 * Draws a rectangle to a SDL surface.
 * 
 * @param dest      Destination surface
 * @param rect      Rectangle to draw
 * @param color     Color to paint
 * @param thickness Thickness of the drawn lines in pixels
 **/

void
SDL_DrawRect(SDL_Surface *dest,
             const SDL_Rect *rect,
             Uint32 color,
             int thickness);

#endif