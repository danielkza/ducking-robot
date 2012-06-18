#include <SDL.h>

#include "video.h"

static int windowed_width = VIDEO_DEFAULT_WIDTH;
static int windowed_height = VIDEO_DEFAULT_HEIGHT;
static int is_fullscreen = 0;

int
video_set_windowed_res(int width,
                       int height)
{
    if(width <= 0 || height <= 0)
        return -1;

    windowed_width = width;
    windowed_height = height;

    return 0;
}

int
video_init(int fullscreen)
{
    return 0;
}
