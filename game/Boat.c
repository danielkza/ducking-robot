#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL.h>

#include "assets.h"
#include "utils.h"
#include "pause.h"

#include "Enemy.h"
#include "Boat.h"

const ent_class_t Boat_CLASS = {
    "Boat", sizeof(Boat), &VisibleEnt_CLASS,
    Boat_m_init, Boat_m_destroy
};

void Boat_m_init(Ent *ent)
{
    Boat *boat = (Boat*)ent;
    const asset_data_t *image = assets_load(ASSET_TYPE_SURFACE, "images/boat-sheet.tga");
    VisibleEnt_SET(image, ent, image->surface);
    Boat_SET(image_index, ent, -1);
    Boat_SET(last_rot_time, ent, 0);

    boat->m_spawn = Boat_m_spawn;
    boat->m_remove = Boat_m_remove;
    boat->m_think = Boat_m_think;
    boat->m_touch = Boat_m_touch;
    boat->m_draw  = Boat_m_draw;
}

void Boat_m_destroy(Ent *ent)
{
}

void Boat_m_spawn(Ent *ent)
{
    Ent_SET(speed, ent, 0);
    Ent_SET(max_speed, ent, 750);
    Ent_SET(flags, ent, Ent_GET(flags, ent) | EFLAGS_VISIBLE | EFLAGS_TOUCHABLE | EFLAGS_SOLID);
    Ent_CALL(think, ent);
}

void Boat_m_remove(Ent *ent)
{
    Ent_SET(flags, ent, Ent_GET(flags, ent) & ~(EFLAGS_VISIBLE | EFLAGS_TOUCHABLE | EFLAGS_SOLID));
}

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
    int row, col, new_image_index;

    float angle = angle_normalize(Ent_GET(rotation, boat));
    if(angle < 0)
        angle = 360 + angle;

    new_image_index = (int)round_float(angle / BOAT_SPRITE_ANGLE_INTERVAL);
    if(new_image_index == Boat_GET(image_index, boat))
        return;
    
    row = new_image_index / BOAT_SPRITE_COLS;
    col = new_image_index % BOAT_SPRITE_COLS;
    rect.x = col * BOAT_SPRITE_SIZE;
    rect.y = row * BOAT_SPRITE_SIZE;

    VisibleEnt_SET(image_rect, boat, &rect);
    Boat_SET(image_index, boat, new_image_index);
    Ent_SET(bounds_width, boat, rect.w);
    Ent_SET(bounds_height, boat, rect.h);
}

void Boat_m_think(Ent *ent)
{
    Boat *boat = (Boat*)ent;
    Uint8 *key_state = SDL_GetKeyState(NULL);

    Boat_update_speed(boat, key_state);
    Boat_update_angles(boat, key_state);
    
    Ent_m_think((Ent*)boat);
    Ent_SET(think_interval, boat, 10);
}

void Boat_m_draw(VisibleEnt *ent)
{
    Boat_update_image((Boat*)ent);
    VisibleEnt_m_draw(ent);
}

void Boat_m_touch(Ent *ent1, Ent *ent2)
{
    Boat *boat = (Boat*)ent1;
    if(Ent_GET(flags, ent2) & EFLAGS_SOLID) {
        if(ent_class_is_subclass(ent2->eclass, &Enemy_CLASS)) {
            set_paused(1);
            set_game_over(1);
        } else {
            float dir_angle = vec2_to_angle(Ent_GET(move_direction, boat)),
                  coll_angle;
            vec2 boat_pos = *Ent_GET(position, boat),
                 coll_pos = *Ent_GET(position, ent2);
            vec2 coll_vec = coll_pos;
        
            vec2_sub(&coll_vec, &boat_pos);
            vec2_norm(&coll_vec);

            coll_angle = vec2_to_angle(&coll_vec);

            if(fabs(dir_angle - coll_angle) <= 45)
                Ent_SET(speed, boat, 0);
        }
    }
}

