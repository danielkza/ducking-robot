#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
    Boat_SET(image_index, ent, -1);

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

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#define clamp(a, a_min, a_max) max(min((a), (a_max)), (a_min))

#define BOAT_SPRITE_SIZE 64
#define BOAT_SPRITE_COLS 3

static float
angle_normalize(float angle)
{
    if(angle < 0)
        angle += 360;
    else if(angle >= 360)
        angle -= 360;

    return angle;
}

static float
angle_round(float angle, int mod)
{
    float quot = angle / mod;
    quot = floorf(quot > 0 ? (quot + 0.5) : (quot - 0.5));
    return quot * mod;
}

void Boat_m_think(Ent *ent)
{
    Boat *boat = (Boat*)ent;
    vec2 move_direction = {0, 0};
    float speed = boat->speed;
    float angle = boat->rotation, move_angle;
    float scale = Ent_GET(think_interval, boat) / 1000.0f;
    int image_index = Boat_GET(image_index, boat), new_image_index;

    Uint8 *key_state = SDL_GetKeyState(NULL);

    if(key_state[SDLK_UP]) {
        speed = clamp(speed + 500 * scale, 0, 750);
    } else if(key_state[SDLK_DOWN]) {
        speed = clamp(speed - 1000 * scale, 0, 750);
    } else if(speed > 0) {
        speed = max(speed - 250 * scale, 0);
    } else if(speed < 0) {
        speed = max(speed + 250 * scale, 0);
    }

    if(key_state[SDLK_LEFT])
        angle += 360 * scale;
    else if(key_state[SDLK_RIGHT])
        angle -= 360 * scale;

    angle = angle_normalize(angle);
    move_angle = angle_normalize(angle_round(angle, 30));

    new_image_index = (int)(move_angle / 30);
    if(new_image_index != image_index) {
        SDL_Rect rect = {0, 0, BOAT_SPRITE_SIZE, BOAT_SPRITE_SIZE};
        int row, col;

        row = new_image_index / BOAT_SPRITE_COLS;
        col = new_image_index % BOAT_SPRITE_COLS;
        rect.x = col * BOAT_SPRITE_SIZE;
        rect.y = row * BOAT_SPRITE_SIZE;
        VisibleEnt_SET(image_rect, boat, &rect);
        Boat_SET(image_index, boat, new_image_index);
    }

    vec2_from_angle(&move_direction, move_angle);
    Ent_SET(move_direction, boat, &move_direction);
    Ent_SET(speed, boat, speed);
    Ent_SET(rotation, boat, angle);

    Ent_m_think((Ent*)boat);

    Ent_SET(think_interval, boat, 20);
}