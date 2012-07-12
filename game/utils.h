#ifndef UTILS_H
#define UTILS_H

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#define clamp(a, a_min, a_max) \
    max(min((a), (a_max)), (a_min))

float
angle_normalize(float angle);

float
angle_round(float angle, int mod);

#endif