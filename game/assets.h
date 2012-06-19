#ifndef ASSETS_H
#define ASSETS_H

#include <SDL.h>
#include <SDL_mixer.h>

#define ASSET_MAX 1024

typedef enum {
    ASSET_TYPE_NONE = 0,
    ASSET_TYPE_SURFACE,
	ASSET_TYPE_ANIMATION,
    ASSET_TYPE_WAVE,
    ASSET_TYPE_MUSIC,
    ASSET_TYPE_BINARY
} asset_type_t;

typedef struct {
    const void *data;
    size_t len;
} asset_binary_data_t;

typedef union {
    SDL_Surface *surface;
    Mix_Chunk *wave;
    Mix_Music *music;
    asset_binary_data_t *bin;
} asset_data_t;

int
assets_init();

void
assets_shutdown();

const asset_data_t *
assets_load(asset_type_t type, const char *file);

#endif
