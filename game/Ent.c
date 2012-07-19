#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "vec2.h"
#include "events.h"
#include "utils.h"
#include "ent_table.h"

#include "Ent.h"
#include "VisibleEnt.h"
#include "Enemy.h"

const ent_class_t Ent_CLASS = {
    "Ent", sizeof(Ent), NULL,
    Ent_m_init, Ent_m_destroy
};

void
Ent_m_init(Ent *ent)
{
    ent->parent = NULL;

    ent->flags = 0;
    
    ent->position = VEC2_ZERO;
    ent->move_direction.x = 1;
    ent->move_direction.y = 0;

    ent->speed = 0.0;
    ent->max_speed = -1.0;
    
    ent->rotation = 0.0;
    ent->rot_speed = 0.0;
    ent->max_rot_speed = -1;0;

    ent->prev_think = 0;
    ent->next_think = 0;

    ent->bounds_width = 0;
    ent->bounds_height = 0;
    
    ent->bounds_rect.x = 0;
    ent->bounds_rect.y = 0;
    ent->bounds_rect.h = 0;
    ent->bounds_rect.w = 0;
    
    ent->m_spawn = Ent_m_spawn;
    ent->m_remove = Ent_m_remove;
    ent->m_think = Ent_m_think;
    ent->m_touch = Ent_m_touch;
    ent->m_on_update = Ent_m_on_update;
    ent->m_on_frame = Ent_m_on_frame;
}

void
Ent_m_destroy(Ent *ent)
{
}

void
Ent_m_spawn(Ent *ent)
{
}

void
Ent_m_remove(Ent *ent)
{
}

void
Ent_m_think(Ent *ent)
{
}

void
Ent_m_touch(Ent *ent, Ent *ent2)
{
}

void
Ent_m_on_update(Ent *ent)
{
}

void
Ent_m_on_frame(Ent *ent)
{
}

static void
Ent_update_position(Ent *ent, float scale)
{
    float speed = Ent_GET(speed, ent),
          max_speed = Ent_GET(max_speed, ent);
    unsigned int flags = Ent_GET(flags, ent);

    if(max_speed >= 0) {
        float speed_abs = fabsf(speed);
        
        if(speed_abs > max_speed) {
            speed = (speed > 0) ? max_speed : -max_speed;
            Ent_SET(speed, ent, speed);
        }
    }

    ent->position.x += (ent->move_direction.x * speed * scale);
    ent->position.y += (ent->move_direction.y * speed * scale);

    // TODO: Bounds for entity move ment
}

static void
Ent_update_rotation(Ent *ent, float scale)
{
    float rotation = Ent_GET(rotation, ent),
          rot_speed = Ent_GET(rot_speed, ent),
          max_rot_speed = Ent_GET(max_rot_speed, ent);

    if(max_rot_speed > 0) {
        float rot_speed_abs = fabsf(rot_speed);
        if(rot_speed_abs > max_rot_speed) {
            rot_speed = (rot_speed > 0) ? max_rot_speed : -max_rot_speed;
            Ent_SET(rot_speed, ent, rot_speed);
        }
    }

    Ent_SET(rotation, ent, angle_normalize(rotation + rot_speed));
}

/*
static void
Ent_update_bounds(Ent *ent)
{
    SDL_Rect bounds;

    vec2 position = *Ent_GET(position, ent);
    float bounds_width = Ent_GET(bounds_width, ent),
          bounds_height = Ent_GET(bounds_height, ent);

    float angle = angle_to_rads(Ent_GET(rotation, ent));

    // Since the rotated bounding rect. is still simetric, we can simply use the absolute values
    // for the sine and cosine to calculate the new bounds.
    float angle_sin = fabsf(sinf(angle)), angle_cos = fabsf(cosf(angle));

    float width_x, height_x, width_final;
    float width_y, height_y, height_final;

    width_x = angle_cos * bounds_width;
    height_x = angle_cos * bounds_height;
    width_final = max(width_x, height_x);
    
    width_y = angle_sin * bounds_width;
    height_y = angle_sin * bounds_height;
    height_final = max(width_x, height_x);

    bounds.x = (int)(position.x - (width_final / 2));
    bounds.y = (int)(position.y - (height_final / 2));
    bounds.w = (unsigned int)width_final;
    bounds.h = (unsigned int)height_final;

    Ent_SET(bounds_rect, ent, &bounds);
}
*/

static void
Ent_update_bounds(Ent *ent)
{
    SDL_Rect bounds;
    const vec2 *position = Ent_GET(position, ent);
    int width = (int)round_float(Ent_GET(bounds_width, ent)),
        height = (int)round_float(Ent_GET(bounds_height, ent));

    bounds.x = (int)round_float(position->x);
    bounds.y = (int)round_float(position->y);
    bounds.w = width;
    bounds.h = height;

    Ent_SET(bounds_rect, ent, &bounds);
}

static int
SDL_IntersectRects(const SDL_Rect *src1,
                   const SDL_Rect *src2,
                   SDL_Rect *dest)
{
    SDL_Rect result;
    const SDL_Rect *r_left, *r_right,
                   *r_top, *r_bottom;
    int empty = 0;

    if(src1 == NULL || src2 == NULL)
        return 0;

    if(src1->x < src2->x) {
        r_left = src1;
        r_right = src2;
    } else {
        r_left = src2;
        r_right = src1;
    }

    result.x = r_right->x;
    result.w = max(0, min(r_right->w, (r_left->x + r_left->w) - r_right->x));

    if(src1->y < src2->y) {
        r_top = src1;
        r_bottom = src2;
    } else {
        r_top = src2;
        r_bottom = src1;
    }

    result.y = r_bottom->y;
    result.h = max(0, min(r_bottom->h, (r_top->y + r_top->h) - r_bottom->y));

    if(dest != NULL)
        *dest = result;
    
    return (result.w != 0 && result.h != 0);
}

static int
Ent_check_single_collision(Ent *ent1, Ent* ent2)
{
    SDL_Rect intersection;
   
    if(ent1 == ent2)
        return 0;
    
    if(SDL_IntersectRects(Ent_GET(bounds_rect, ent1), Ent_GET(bounds_rect, ent2), &intersection) != 0) {
        SDL_Surface *screen = SDL_GetVideoSurface();
        //SDL_DrawRect(screen, &intersection, 0xFF00FFFF, 10);
        
        if(ent_class_is_subclass(ent1->eclass, &VisibleEnt_CLASS)) {
            if(!VisibleEnt_check_single_collision((VisibleEnt*)ent1, ent2,
                                                  &intersection))
            {
                return 0;
            }
        } else if(ent_class_is_subclass(ent2->eclass, &VisibleEnt_CLASS)) {
            if(!VisibleEnt_check_single_collision((VisibleEnt*)ent2,
                                                  ent1, &intersection))
            {
                return 0;
            }
        }

        Ent_CALL(touch, ent1, ent2);
        Ent_CALL(touch, ent2, ent1);

        return 1;
    }

    return 0;
}

static int
Ent_check_collisions(Ent *ent)
{
    const list_t *ent_iter = NULL;
    int has_collided = 0;
    
    if(!(Ent_GET(flags, ent) & EFLAGS_TOUCHABLE))
        return 0;

    while((ent_iter = ent_table_next(ent_iter)) != NULL) {
        Ent *coll_ent = ent_iter->item;
        
        if(coll_ent == ent)
            continue;

        if(!(Ent_GET(flags, ent) & EFLAGS_TOUCHABLE))
            continue;

        if(Ent_check_single_collision(ent, coll_ent))
            has_collided = 1;
    }

    return has_collided;
}

void
Ent_update(Ent *ent, Uint32 last_frame_time)
{
    vec2 old_position = *Ent_GET(position, ent);
    float old_rotation = Ent_GET(rotation, ent);
    float scale = (game_time() - last_frame_time) / 1000.0f;

    Ent_update_position(ent, scale);
    Ent_update_rotation(ent, scale);
    Ent_update_bounds(ent);

    if(Ent_check_collisions(ent) && !ent_class_is_subclass(ent->eclass, &Enemy_CLASS)) {
        Ent_SET(position, ent, &old_position);
        Ent_SET(rotation, ent, old_rotation);
        Ent_update_bounds(ent);
    }
    
    Ent_CALL(on_update, ent);
}
