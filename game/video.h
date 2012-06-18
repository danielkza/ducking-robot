#ifndef VIDEO_H
#define VIDEO_H

#include <SDL.h>

#define VIDEO_DEFAULT_WIDTH 1024
#define VIDEO_DEFAULT_HEIGHT 768

int
video_set_windowed_res(int width,
                       int height);

int
video_init(int fullscreen);

int
video_shutdown();

#endif