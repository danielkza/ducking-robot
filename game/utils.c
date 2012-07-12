#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "utils.h"

float
angle_normalize(float angle)
{
    if(angle < -180)
        angle += 360;
    else if(angle >= 180)
        angle -= 360;

    return angle;
}

float
angle_round(float angle, int mod)
{
    float quot = angle / mod;
    quot = floorf(quot > 0 ? (quot + 0.5) : (quot - 0.5));
    return quot * mod;
}