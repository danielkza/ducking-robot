#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "hash_fnv1a.h"
#include "assets.h"

#ifdef _MSC_VER
#define strcasecmp _stricmp
#endif

static struct {
    char *file;
    asset_type_t type;
    asset_data_t data;
} asset_table[ASSET_MAX];

static int asset_table_count = -1;

int
assets_init()
{
    asset_table_count = 0;

    IMG_Init(IMG_INIT_PNG);

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
    case ASSET_TYPE_ANIMATION:
        // TODO: implement animation freeing
        break;
    case ASSET_TYPE_WAVE:
        Mix_FreeChunk(data->wave);
        break;
    case ASSET_TYPE_MUSIC:
        Mix_FreeMusic(data->music);
        break;
    case ASSET_TYPE_BINARY:
        free((void*)data->bin->data);
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
    unsigned int hash = assets_hash_string(file) % ASSET_MAX,
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
              const asset_data_t *data)
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
    asset_table[pos].data = *data;

    return 1;
}

static asset_binary_data_t *
assets_load_binary(const char *file)
{
    struct stat finfo;
    FILE *fp;
    asset_binary_data_t *bin_data;
    void *data;

    if(stat(file, &finfo) != 0)
        return NULL;

    fp = fopen(file, "rb");
    if(fp == NULL)
        return NULL;

    bin_data = malloc(sizeof(*bin_data));
    if(bin_data == NULL) {
        fclose(fp);
        return NULL;
    }
    
    data = malloc(bin_data->len);
    if(data == NULL
       || (bin_data->len = fread(data, 1, bin_data->len, fp)) <= 0)
    {
        fclose(fp);
        free(data);
        free(bin_data);
        return NULL;
    }

    bin_data->data = data;

    fclose(fp);
    return bin_data;
}

const asset_data_t *
assets_load(asset_type_t type, const char *file)
{
    int pos;
    asset_data_t data;

    if(file == NULL || file[0] == '\0')
        return NULL;

    pos = assets_find_pos(file);
    if(pos < 0)
        return NULL;

    if(asset_table[pos].file != NULL) {
        if(type != asset_table[pos].type)
            return NULL;
        
        return &asset_table[pos].data;
    }

    // Asset not yet loaded, read it from file and cache it
    switch(type) {
    case ASSET_TYPE_SURFACE:
        data.surface = IMG_Load(file);
        break;
    //case ASSET_TYPE_ANIMATION:
        // TODO: load animation
        // break;
    case ASSET_TYPE_WAVE:
        data.wave = Mix_LoadWAV(file);
        break;
    case ASSET_TYPE_MUSIC:
        data.music = Mix_LoadMUS(file);
        break;
    case ASSET_TYPE_BINARY:
        data.bin = assets_load_binary(file);
        break;
    default:
        return NULL;
    }

    asset_table[pos].data = data;
    return &asset_table[pos].data;
}