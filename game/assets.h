#ifndef ASSETS_H
#define ASSETS_H

#include <SDL.h>

typedef struct {
    void *data;
    size_t len;
} asset_binary_data_t;

typedef union {
    SDL_Surface *surface;
    Mix_Chunk *wave;
    Mix_Music *music;
    asset_binary_data_t *bin;
} asset_data_t;

asset_handle_t
assets_load(const char *file);

void
assets_release(asset_handle_t asset);

SDL_RWops *
assets_data(asset_handle_t asset);



#endif