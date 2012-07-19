/**
  * @file
  * Enemy entity class
  **/

#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>

#include "Ent.h"
#include "VisibleEnt.h"

/** Each image on the sprite sheet corresponds to this interval, in degrees
    of the rotation. **/
#define ENEMY_SPRITE_ANGLE_INTERVAL 30
/** Width and height, in pixels, of each sprite **/
#define ENEMY_SPRITE_SIZE 64
/** Number of sprites in each row in the sprite sheet **/
#define ENEMY_SPRITE_COLS 3
/** How often, in miliseconds, to rotate the boat when the rotation key is
    being pressed **/

const ent_class_t Enemy_CLASS;

#define Enemy_STRUCT \
    VisibleEnt_STRUCT; \
    int image_index; \
    Uint32 last_rot_time

typedef struct Enemy {
    Enemy_STRUCT;
} Enemy;

CLS_DEF_ACCESSOR(Enemy, int, image_index);
CLS_DEF_ACCESSOR(Enemy, Uint32, last_rot_time);

#define Enemy_GET(member, obj) \
    CLS_DEF_GET(Enemy, member, obj)

#define Enemy_SET(member, obj, ...) \
    CLS_DEF_SET(Enemy, member, obj, ##__VA_ARGS__)

#define Enemy_CALL(method, obj, ...) \
    CLS_DEF_CALL(Enemy, method, obj, ##__VA_ARGS__)


void Enemy_m_init(Ent *ent);
void Enemy_m_destroy(Ent *ent);
void Enemy_m_spawn(Ent *ent);
void Enemy_m_remove(Ent *ent);
void Enemy_m_think(Ent *ent);
void Enemy_m_touch(Ent *ent1, Ent *ent2);
void Enemy_m_draw(VisibleEnt *ent);
void Enemy_m_touch(Ent *ent1, Ent *ent2);

#endif