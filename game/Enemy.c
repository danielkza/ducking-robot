#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL.h>

#include "assets.h"
#include "utils.h"

#include "Enemy.h"

const ent_class_t Enemy_CLASS = {
    "Enemy", sizeof(Enemy), &VisibleEnt_CLASS,
    Enemy_m_init, Enemy_m_destroy
};

void Enemy_m_init(Ent *ent)
{
    Enemy *enemy = (Enemy*)ent;
    const asset_data_t *image = assets_load(ASSET_TYPE_SURFACE, "images/enemy-sheet.tga");
    VisibleEnt_SET(image, enemy, image->surface);
    Enemy_SET(image_index, enemy, -1);
    Enemy_SET(last_rot_time, enemy, 0);

    enemy->m_spawn = Enemy_m_spawn;
    enemy->m_remove = Enemy_m_remove;
    enemy->m_think = Enemy_m_think;
    enemy->m_touch = Enemy_m_touch;
    enemy->m_draw  = Enemy_m_draw;
}

void Enemy_m_destroy(Ent *ent)
{
}

void Enemy_m_spawn(Ent *ent)
{
    Ent_SET(speed, ent, 0);
    Ent_SET(max_speed, ent, 750);
    Ent_SET(flags, ent, Ent_GET(flags, ent) | EFLAGS_VISIBLE | EFLAGS_TOUCHABLE | EFLAGS_SOLID);
    Ent_CALL(think, ent);
}

void Enemy_m_remove(Ent *ent)
{
    Ent_SET(flags, ent, Ent_GET(flags, ent) & ~(EFLAGS_VISIBLE | EFLAGS_TOUCHABLE | EFLAGS_SOLID));
}

static void
Enemy_update_speed(Enemy *boat)
{
    float speed = Ent_GET(speed, boat);
    float scale = Ent_GET(think_interval, boat) / 1000.0f;

    Ent_SET(speed, boat, speed);
}

static void
Enemy_update_angles(Enemy *boat)
{
    float angle = Ent_GET(rotation, boat);
    vec2 move_direction = {0, 0};
 
    angle = angle_normalize(angle_round(angle, ENEMY_SPRITE_ANGLE_INTERVAL));
    Ent_SET(rotation, boat, angle);

    vec2_from_angle(&move_direction, angle);
    Ent_SET(move_direction, boat, &move_direction);
}

static void
Enemy_update_image(Enemy *boat)
{
    SDL_Rect rect = {0, 0, ENEMY_SPRITE_SIZE, ENEMY_SPRITE_SIZE};
    int row, col, new_image_index;

    float angle = angle_normalize(Ent_GET(rotation, boat));
    if(angle < 0)
        angle = 360 + angle;

    new_image_index = (int)(angle / ENEMY_SPRITE_ANGLE_INTERVAL);
    if(new_image_index == Enemy_GET(image_index, boat))
        return;
    
    row = new_image_index / ENEMY_SPRITE_COLS;
    col = new_image_index % ENEMY_SPRITE_COLS;
    rect.x = col * ENEMY_SPRITE_SIZE;
    rect.y = row * ENEMY_SPRITE_SIZE;

    VisibleEnt_SET(image_rect, boat, &rect);
    Enemy_SET(image_index, boat, new_image_index);
    Ent_SET(bounds_width, boat, rect.w);
    Ent_SET(bounds_height, boat, rect.h);
}

void Enemy_m_think(Ent *ent)
{
    Enemy *boat = (Enemy*)ent;
    Uint8 *key_state = SDL_GetKeyState(NULL);

    Enemy_update_speed(boat);
    Enemy_update_angles(boat);
    
    Ent_m_think((Ent*)boat);
    Ent_SET(think_interval, boat, 10);
}

void Enemy_m_draw(VisibleEnt *ent)
{
    Enemy_update_image((Enemy*)ent);
    VisibleEnt_m_draw(ent);
}

void Enemy_m_touch(Ent *ent1, Ent *ent2)
{
    Enemy *enemy = (Enemy*)ent1;
    if(Ent_GET(flags, ent2) & EFLAGS_SOLID) {
        Ent_SET(speed, enemy, 0);
    }
}