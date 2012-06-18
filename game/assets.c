#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "hash_fnv1a.h"
#include "assets.h"

#ifdef _MSC_VER
#define strcasecmp _stricmp
#endif

typedef enum {
    ASSET_TYPE_NONE = 0,
    ASSET_TYPE_SURFACE,
    ASSET_TYPE_WAVE,
    ASSET_TYPE_MUSIC,
    ASSET_TYPE_BINARY
} asset_type_t;

static struct {
    asset_type_t type;
    asset_data_t data;
} asset_table[ASSET_MAX];

static int asset_table_count = -1;

int
assets_init()
{
    return 0;
}

static void
asset_data_free(asset_type_t type, asset_data_t *data)
{
    if(data == NULL)
        return;

    switch(type) {
    case ASSET_TYPE_SURFACE:
        SDL_FreeSurface(data->surface);
        break;
    case ASSET_TYPE_WAVE:
        Mix_FreeChunk(data->wave);
        break;
    case ASSET_TYPE_MUSIC:
        Mix_FreeMusic(data->music);
        break;
    case ASSET_TYPE_BINARY:
        free(data->bin->data);
        free(data->bin);
        break;
    case ASSET_TYPE_NONE:
        break;
    default:
        break;
    }
}


void
assets_shutdown()
{
    if(asset_table_count > 0) {
        int i;
        for(i = 0; i < ASSET_MAX; i++)
        {
            if(asset_table[i].file == NULL)
                continue;

            free(asset_table[i].file);
            asset_data_free(asset_table[i].type, &asset_table[i].data);
        }
    }

    asset_table_count = -1;
}

static unsigned int
assets_hash_string(const char *string)
{
    return hash_fnv1a_str(string);
}

static int
assets_find_pos(const char *file)
{
    unsigned int hash, pos;
    
    hash = assets_hash_string(type) % ASSET_MAX;
    pos = hash;
    
    do
    {
        if(asset_table[pos].file == NULL
           || strcasecmp(file, asset_table[pos].file) == 0)
        {
            return pos;
        }

        pos++;
    } while(pos != hash);

    return -1;
}

static int
assets_insert(int pos,
              const char *file,
              asset_type_t type,
              asset_data_t data)
{
    char *file_copy;
    int file_len;

    if(pos < 0 || file == NULL || file[0] == '\0' || type == ASSET_TYPE_NONE)
        return 0;

    file_len = strlen(file);
    file_copy = malloc(file_len + 1);
    
    assert(file_copy != NULL);
    memcpy(file_copy, file, file_len);
    file_copy[file_len] = '\0';

    asset_table[pos].file = file_copy;
    asset_table[pos].type = type;
    asset_table[pos].data = data;

    return 1;
}

SDL_Surface *
assets_load_image(const char *file)
{
    SDL_Surface *surface
    int pos;
    
    if(file == NULL || file[0] == '\0')
        return NULL;

    pos = assets_find_pos(file);
    if(pos >= 0)
        surface = asset_table[pos].data.surface;
}