/**
 * @file
 * Vector data type.
 **/

#ifndef VEC2_H
#define VEC2_H

#define _USE_MATH_DEFINES
#include <math.h>

#include <utils.h>

#ifdef _MSC_VER
#define inline __inline
#endif

/** Type representing a two-dimensional vector **/
typedef struct {
    /** X-axis **/
    float x;
    /** Y-axis **/
    float y;
} vec2;

/** Constant representing a null vector for convenience **/
static const vec2 VEC2_ZERO = {0, 0};

#define APPLY(dest, src, _op_) \
    (dest)->x = (dest)->x _op_ (src)->x, \
    (dest)->y = (dest)->y _op_ (src)->y

/**
 * Adds two vectors in place. (dest = dest + src)
 *
 * @param dest Destination vector
 * @param src  Source vector
 **/

static inline void
vec2_add(vec2 *dest, const vec2 *src)
{
    APPLY(dest, src, +);
}

/**
 * Subtracts two vectors in place. (dest = dest - src)
 *
 * @param dest Destination vector
 * @param src  Source vector
 **/

static inline void
vec2_sub(vec2 *dest, const vec2 *src)
{
    APPLY(dest, src, -);
}

/**
 * Calculates the scalar product of two vectors in place. (dest = dest . src)
 *
 * @param dest Destination vector
 * @param src  Source vector
 **/

static inline void
vec2_dot(vec2 *dest, const vec2 *src)
{
    APPLY(dest, src, *);
}

#undef APPLY

/**
 * Scales a vector by a scalar in-place.
 *
 * @param dest  Destination vector
 * @param scale Scalar value
 **/

static inline void
vec2_scale(vec2 *dest, float scale)
{
    dest->x *= scale;
    dest->y *= scale;
}

/**
 * Returns the square of the length (magnitude) of a vector.
 * Slightly faster to use over vec2_len() if you were gonna square the result
 * anyway.
 *
 * @param src Source Vector
 *
 * @returns Squared length
 **/

static inline float
vec2_len_sqr(const vec2 *src)
{
    return (src->x * src->x) + (src->y * src->y);
}

/**
 * Returns the length (magnitude) of a vector.
 *
 * @param src Source Vector
 *
 * @returns Length
 **/

static inline float
vec2_len(const vec2 *src)
{
    return (float)sqrt((float)vec2_len_sqr(src));
}

/**
 * Normalizes a vector in place, resulting in a vector of the same direction
 * with length 1.
 *
 * @param src Destination vector
 **/

static inline void
vec2_norm(vec2 *dest)
{
    float len = vec2_len(dest);
    if(len == 0.0f) {
        dest->x = dest->y = 0.0f;
    } else {
        dest->x /= len;
        dest->y /= len;
    }
}

/**
 * Creates a unit vector poiting to a particular direction.
 *
 * @param dest  Destination vector
 * @param angle Angle in degrees
 **/

static inline void
vec2_from_angle(vec2 *dest, float angle)
{
    float angle_rads = angle_to_rads(angle);
    dest->x = cosf(angle_rads);
    dest->y = -sinf(angle_rads);
}


/**
 * Retrieves the direction of a vector in degrees.
 *
 * @param src Source vector
 * @returns Angle in degrees
 **/

static inline float
vec2_to_angle(const vec2 *src)
{
    float angle_rads = atan2f(src->y, src->x);
    return angle_normalize(angle_to_degs(angle_rads));
}

/**
 * Checks if a vector is null (has length 0)
 *
 * @param src Source vector
 *
 * @returns 1 if vector is null, 0 otherwise
 */

static inline int
vec2_is_null(const vec2 *src)
{
    return src->x == 0 && src->y == 0;
}

/**
 * Rotates a vector by a specified angle in degrees.
 *
 * @param dest Destination vector
 * @param angle Angle to rotate in degrees
 **/

static inline void
vec2_rotate(vec2 *dest, float angle)
{
    float rotation_rads = angle_to_rads(angle);
    dest->x += dest->x * cosf(rotation_rads);
    dest->y += dest->y * sinf(rotation_rads);
}

#endif
