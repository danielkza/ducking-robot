#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL.h>

#include "assets.h"
#include "utils.h"

#include "Enemy.h"

void Enemy_m_init(Ent *ent)
{
    const asset_data_t *image = assets_load(ASSET_TYPE_SURFACE, "images/enemy-sheet.tga");
    VisibleEnt_SET(image, ent, image->surface);
    Enemy_SET(image_index, ent, -1);
    Enemy_SET(last_rot_time, ent, 0);

    ent->m_spawn = Enemy_m_spawn;
    ent->m_think = Enemy_m_think;
}

void Enemy_m_spawn(Ent *ent)
{
    Ent_SET(speed, ent, 2);
    Ent_SET(max_speed, ent, 750);

    Ent_CALL(think, ent);
}

static void
Enemy_update_image(Enemy *Enemy)
{
    SDL_Rect rect = {0, 0, Enemy_SPRITE_SIZE, Enemy_SPRITE_SIZE};
    int row, col, new_image_index;

    float angle = angle_normalize(Ent_GET(rotation, Enemy));
    if(angle < 0)
        angle = 360 + angle;

    new_image_index = (int)(angle / Enemy_SPRITE_ANGLE_INTERVAL);
    if(new_image_index == Enemy_GET(image_index, Enemy))
        return;
    
    row = new_image_index / Enemy_SPRITE_COLS;
    col = new_image_index % Enemy_SPRITE_COLS;
    rect.x = col * Enemy_SPRITE_SIZE;
    rect.y = row * Enemy_SPRITE_SIZE;

    VisibleEnt_SET(image_rect, Enemy, &rect);
    Enemy_SET(image_index, Enemy, new_image_index);
}

void Enemy_m_think(Ent *ent)
{
    Enemy *Enemy = (Enemy*)ent;

    Enemy_update_image(Enemy);

    Ent_m_think((Ent*)Enemy);
    Ent_SET(think_interval, Enemy, 1);
}