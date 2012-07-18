#ifndef VEC2_H
#define VEC2_H

#define _USE_MATH_DEFINES
#include <math.h>

#include <utils.h>

#ifdef _MSC_VER
#define inline __inline
#endif

typedef struct {
    float x, y;
} vec2;

static const vec2 VEC2_ZERO = {0, 0};

#define APPLY(dest, src, _op_) \
    (dest)->x = (dest)->x _op_ (src)->x, \
    (dest)->y = (dest)->y _op_ (src)->y

static inline void
vec2_add(vec2 *dest, const vec2 *src)
{
    APPLY(dest, src, +);
}

static inline void
vec2_sub(vec2 *dest, const vec2 *src)
{
    APPLY(dest, src, -);
}

static inline void
vec2_dot(vec2 *dest, const vec2 *src)
{
    APPLY(dest, src, *);
}

#undef APPLY

static inline void
vec2_scale(vec2 *dest, float scale)
{
    dest->x *= scale;
    dest->y *= scale;
}

static inline float
vec2_len_sqr(const vec2 *src)
{
    return (src->x * src->x) + (src->y * src->y);
}

static inline float
vec2_len(const vec2 *src)
{
    return (float)sqrt((float)vec2_len_sqr(src));
}

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

static inline void
vec2_from_angle(vec2 *dest, float angle)
{
    float angle_rads = angle_to_rads(angle);
    dest->x = cosf(angle_rads);
    dest->y = -sinf(angle_rads);
}

static inline float
vec2_to_angle(const vec2 *src)
{
    float angle_rads = atan2f(src->y, src->x);
    return angle_normalize(angle_to_degs(angle_rads));
}

static inline int
vec2_is_null(const vec2 *src)
{
    return src->x == 0 && src->y == 0;
}

static inline void
vec2_rotate(vec2 *dest, float rotation)
{
    double rotation_rads = angle_to_rads(rotation);
    dest->x += dest->x * cosf(rotation_rads);
    dest->y += dest->y * sinf(rotation_rads);
}

#endif
