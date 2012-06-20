#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#include "assets.h"
#include "Boat.h"

const ent_class_t Boat_CLASS = {
    "Boat", sizeof(Boat), &VisibleEnt_CLASS,
    Boat_m_init, Boat_m_destroy
};

void Boat_m_init(Ent *ent)
{
    const asset_data_t *image = assets_load(ASSET_TYPE_SURFACE, "images/boat-sheet.tga");
    VisibleEnt_SET(image, ent, image->surface);
    Boat_SET(image_index, ent, 0);

    ent->m_spawn = Boat_m_spawn;
    ent->m_think = Boat_m_think;
}

void Boat_m_destroy(Ent *ent)
{
}

void Boat_m_spawn(Ent *ent)
{
    Ent_CALL(think, ent);
}

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define clamp(a, a_min, a_max) max(min((a), (a_max)), (a_min))

#define BOAT_SPRITE_SIZE 64
#define BOAT_SPRITE_ROWS 4

void Boat_m_think(Ent *ent)
{
    Boat *boat = (Boat*)ent;
    vec2 move_direction = {0, 0};
    float speed = boat->speed;
    float angle = boat->rotation;
    float scale = Ent_GET(think_interval, boat) / 1000.0f;
    int image_index = Boat_GET(image_index, boat), new_image_index;

    Uint8 *key_state = SDL_GetKeyState(NULL);

    if(speed > 0) {
        speed = max(speed - 100 * scale, 0);
    } else if(speed < 0) {
        speed = max(speed + 100 * scale, 0);
    }

    if(key_state[SDLK_UP]) {
        speed = clamp(speed + 150 * scale, 0, 250);
    } else if(key_state[SDLK_DOWN]) {
        speed = clamp(speed - 150 * scale, 0, 250);
    }

    if(key_state[SDLK_LEFT])
        angle = (float)fmod(angle - 180 * scale, 360);
    else if(key_state[SDLK_RIGHT])
        angle = (float)fmod(angle + 180 * scale, 360);

    new_image_index = (int)(angle / 30);
    if(new_image_index != image_index) {
        SDL_Rect rect = {0, 0, BOAT_SPRITE_SIZE, BOAT_SPRITE_SIZE};
        int row, col;

        row = new_image_index / BOAT_SPRITE_ROWS;
        col = new_image_index % BOAT_SPRITE_ROWS;
        rect.x = col * BOAT_SPRITE_SIZE;
        rect.y = row * BOAT_SPRITE_SIZE;
        VisibleEnt_SET(image_rect, boat, &rect);
    }

    vec2_from_angle(&move_direction, angle);
    Ent_SET(move_direction, boat, &move_direction);
    Ent_SET(speed, boat, speed);
    Ent_SET(rotation, boat, angle);

    Ent_CALL(think, ent);

    Ent_SET(think_interval, boat, 20);
}