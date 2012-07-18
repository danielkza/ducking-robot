#ifndef ASSETS_H
#define ASSETS_H

#include <SDL.h>
#include <SDL_mixer.h>

/** Maximum number of assets that can be loaded (size of the asset table) **/
#define ASSET_MAX 1024

/** Types of assets that can be loaded **/
typedef enum {
    /** Invalid asset type **/
    ASSET_TYPE_NONE = 0,
    /** Images/textures loaded by SDL_image **/
    ASSET_TYPE_SURFACE,
    /** Sets of images to be used on animation **/
    ASSET_TYPE_ANIMATION,
    /** Plain sound files for SFX **/
    ASSET_TYPE_WAVE,
    /** Music files to be used as BGM **/
    ASSET_TYPE_MUSIC,
    /** Generic binary data **/
    ASSET_TYPE_BINARY
} asset_type_t;

/** Structure for loading of binary data as assets **/
typedef struct {
    /** Pointer to loaded data **/
    const void *data;
    /** Length of the data in bytes **/
    size_t len;
} asset_binary_data_t;

/** Union containing all the possible asset data types **/
typedef union {
    /** SDL surface (textures and images) **/
    SDL_Surface *surface;
    /** Wave file (SFX) **/
    Mix_Chunk *wave;
    /** Music file (BGM) **/
    Mix_Music *music;
    /** Generic binary data **/
    asset_binary_data_t *bin;
} asset_data_t;

/**
 * Initializes internal data for the asset system. Must be called before using any other facility
 **/
int
assets_init();

/**
 * Invalidates all internal state and frees up any loaded data.
 * assets_init() must be called again after calling this function for further use.
 *
 * @warning does not do reference counting. Calling will immediately invalide all resources.
 **/
void
assets_shutdown();

/**
 * Loads an asset from a file, or if available, from a cached version instead.
 *
 * @warning Any caller trying to load the same asset will receive a shared copy.
            DO NOT modify contents directly; create a copy of them first or you may encounter
            unexpected behavior.
 * @param type Type of asset to load from file.
 * @param file Name of the file to load from. Will be used as the key for caching.
 *
 * @return Asset data (which may have NULL members on error), or a NULL pointer on failure.
 **/

const asset_data_t *
assets_load(asset_type_t type, const char *file);

#endif
