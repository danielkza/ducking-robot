#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL.h>

#include "assets.h"
#include "utils.h"
#include "ent_table.h"

#include "Boat.h"
#include "Enemy.h"

const ent_class_t Enemy_CLASS = {
    "Enemy", sizeof(Enemy), &VisibleEnt_CLASS,
    Enemy_m_init, Enemy_m_destroy
};

static Boat *player_boat = NULL;

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
    const SDL_VideoInfo *video_info = SDL_GetVideoInfo();
    vec2 position;

    position.x = (float)(rand() % video_info->current_w);
    position.y = (float)(rand() % video_info->current_h);
    Ent_SET(position, ent, &position);

    Ent_SET(speed, ent, 0);
    Ent_SET(max_speed, ent, 750);
    Ent_SET(flags, ent, Ent_GET(flags, ent) | EFLAGS_VISIBLE | EFLAGS_TOUCHABLE | EFLAGS_SOLID);
    
    if(player_boat == NULL) {
        const list_t *boat_iter = ent_table_next_by_class(NULL, &Boat_CLASS, 0);
        if(boat_iter != NULL)
            player_boat = boat_iter->item;
    }

    Ent_CALL(think, ent);
}

void Enemy_m_remove(Ent *ent)
{
    Ent_SET(flags, ent, Ent_GET(flags, ent) & ~(EFLAGS_VISIBLE | EFLAGS_TOUCHABLE | EFLAGS_SOLID));
}

static void
Enemy_update_speed_angles(Enemy *enemy)
{
    float scale = Ent_GET(think_interval, enemy) / 1000.0f;
    const vec2 *position = Ent_GET(position, enemy);
    float speed = Ent_GET(speed, enemy);
    vec2 velocity = *Ent_GET(move_direction, enemy);
    
    vec2 player_pos = *Ent_GET(position, player_boat);
    vec2 player_vec = player_pos;

    vec2_scale(&velocity, speed - (250 * scale));

    vec2_sub(&player_vec, position);
    vec2_norm(&player_vec);

    vec2_scale(&player_vec, 500 * scale);
    vec2_add(&velocity, &player_vec);
    
    Ent_SET(rotation, enemy, vec2_to_angle(&velocity));
    Ent_SET(velocity_vec, enemy, &velocity);
}

static void
Enemy_update_image(Enemy *enemy)
{
    SDL_Rect rect = {0, 0, ENEMY_SPRITE_SIZE, ENEMY_SPRITE_SIZE};
    int row, col, new_image_index;

    float angle = angle_normalize(Ent_GET(rotation, enemy));
    if(angle < 0)
        angle += 360;

    new_image_index = (int)round_float(angle / ENEMY_SPRITE_ANGLE_INTERVAL);
    if(new_image_index == Enemy_GET(image_index, enemy))
        return;
    
    row = new_image_index / ENEMY_SPRITE_COLS;
    col = new_image_index % ENEMY_SPRITE_COLS;
    rect.x = col * ENEMY_SPRITE_SIZE;
    rect.y = row * ENEMY_SPRITE_SIZE;

    VisibleEnt_SET(image_rect, enemy, &rect);
    Enemy_SET(image_index, enemy, new_image_index);
    Ent_SET(bounds_width, enemy, rect.w);
    Ent_SET(bounds_height, enemy, rect.h);
}

void Enemy_m_think(Ent *ent)
{
    Enemy *enemy = (Enemy*)ent;
    Uint8 *key_state = SDL_GetKeyState(NULL);

    Enemy_update_speed_angles(enemy);
    
    Ent_m_think((Ent*)enemy);
    Ent_SET(think_interval, enemy, 10);
}

void Enemy_m_draw(VisibleEnt *ent)
{
    Enemy_update_image((Enemy*)ent);
    VisibleEnt_m_draw(ent);
}

void Enemy_m_touch(Ent *ent1, Ent *ent2)
{
    /*
    Enemy *enemy = (Enemy*)ent1;
    if(Ent_GET(flags, ent2) & EFLAGS_SOLID) {
        float dir_angle = vec2_to_angle(Ent_GET(move_direction, enemy)),
              coll_angle;
        vec2 enemy_pos = *Ent_GET(position, enemy),
             coll_pos = *Ent_GET(position, ent2);
        vec2 coll_vec = coll_pos;

        vec2_sub(&coll_vec, &enemy_pos);
        vec2_norm(&coll_vec);

        coll_angle = vec2_to_angle(&coll_vec);

        if(fabs(dir_angle - coll_angle) <= 45)
            Ent_SET(speed, enemy, 0);
    }
    */
}