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
    Boat_SET(last_rot_time, ent, 0);

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

#define BOAT_SPRITE_ANGLE_INTERVAL 30
#define BOAT_SPRITE_SIZE 64
#define BOAT_SPRITE_COLS 3
#define BOAT_ROTATION_DELAY 100

static void
Boat_update_speed(Boat *boat, const Uint8 *key_state)
{
    float speed = Ent_GET(speed, boat);
    float scale = Ent_GET(think_interval, boat) / 1000.0f;
  
    if(key_state[SDLK_UP]) {
        speed = clamp(speed + 500 * scale, 0, 750);
    } else if(key_state[SDLK_DOWN]) {
        speed = clamp(speed - 1000 * scale, 0, 750);
    } else if(speed > 0) {
        speed = max(speed - 250 * scale, 0);
    } else if(speed < 0) {
        speed = max(speed + 250 * scale, 0);
    }

    Ent_SET(speed, boat, speed);
}

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

static void
Boat_update_angles(Boat *boat, const Uint8 *key_state)
{
    Uint32 cur_time = game_time(),
           last_rot_time = Boat_GET(last_rot_time, boat);
    float angle = Ent_GET(rotation, boat);
    vec2 move_direction = {0, 0};
 
    if(!key_state[SDLK_RIGHT] && !key_state[SDLK_LEFT]) {
        Boat_SET(last_rot_time, boat, 0);
        return;
    }

    if(last_rot_time != 0 && (cur_time - last_rot_time) < BOAT_ROTATION_DELAY)
        return;

    if(key_state[SDLK_LEFT])
        angle += BOAT_SPRITE_ANGLE_INTERVAL;
    else
        angle -= BOAT_SPRITE_ANGLE_INTERVAL;

    Boat_SET(last_rot_time, boat, cur_time);

    angle = angle_normalize(angle_round(angle, BOAT_SPRITE_ANGLE_INTERVAL));
    Ent_SET(rotation, boat, angle);

    vec2_from_angle(&move_direction, angle);
    Ent_SET(move_direction, boat, &move_direction);
}

static void
Boat_update_image(Boat *boat)
{
    SDL_Rect rect = {0, 0, BOAT_SPRITE_SIZE, BOAT_SPRITE_SIZE};
    int row, col;

    int new_image_index = (int)(Ent_GET(rotation, boat) / BOAT_SPRITE_ANGLE_INTERVAL);
    if(new_image_index == Boat_GET(image_index, boat))
        return;
    
    row = new_image_index / BOAT_SPRITE_COLS;
    col = new_image_index % BOAT_SPRITE_COLS;
    rect.x = col * BOAT_SPRITE_SIZE;
    rect.y = row * BOAT_SPRITE_SIZE;

    VisibleEnt_SET(image_rect, boat, &rect);
    Boat_SET(image_index, boat, new_image_index);
}

void Boat_m_think(Ent *ent)
{
    Boat *boat = (Boat*)ent;
    Uint8 *key_state = SDL_GetKeyState(NULL);

    Boat_update_speed(boat, key_state);
    Boat_update_angles(boat, key_state);
    Boat_update_image(boat);

    Ent_m_think((Ent*)boat);
    Ent_SET(think_interval, boat, 1);
}