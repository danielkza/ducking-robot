#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "vec2.h"
#include "events.h"
#include "utils.h"

#include "Ent.h"

const ent_class_t Ent_CLASS = {
    "Ent", sizeof(Ent), NULL,
    Ent_m_init, Ent_m_destroy
};

static void
Ent_create_init(Ent *ent, const ent_class_t *eclass)
{
    if(eclass->base != NULL)
        Ent_create_init(ent, eclass->base);

    (*eclass->m_init)(ent);
}

Ent *
Ent_m_create(const ent_class_t *eclass)
{
    Ent *ent;
    
    if(eclass == NULL)
        return NULL;

    ent = malloc(eclass->data_size);
    if(ent == NULL)
        return NULL;

    ent->eclass = eclass;
    Ent_create_init(ent, eclass);
 
    return ent;
}

static void
Ent_free_destroy(Ent *ent, const ent_class_t *eclass)
{
    (*eclass->m_destroy)(ent);

    if(eclass->base != NULL)
        Ent_free_destroy(ent, eclass->base);
}

void
Ent_m_free(Ent *ent)
{
    if(ent != NULL) {
        Ent_free_destroy(ent, ent->eclass);
        free(ent);
    }
}

void
Ent_m_init(Ent *ent)
{
    ent->parent = NULL;

    ent->flags = 0;
    
    ent->position = VEC2_ZERO;
    ent->move_direction = VEC2_ZERO;
    ent->speed = 0.0;
    ent->max_speed = -1.0;
    
    ent->rotation = 0.0;
    ent->rot_speed = 0.0;
    ent->max_rot_speed = -1;0;

    ent->prev_think = 0;
    ent->next_think = 0;

    ent->m_spawn = Ent_m_spawn;
    ent->m_remove = Ent_m_remove;
    ent->m_think = Ent_m_think;
    ent->m_touch = Ent_m_touch;
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

static void
Ent_update_position(Ent *ent, float scale)
{
    float speed = Ent_GET(speed, ent),
          max_speed = Ent_GET(max_speed, ent),
          speed_scaled;
    unsigned int flags = Ent_GET(flags, ent);

    if(max_speed >= 0) {
        float speed_abs = fabs(speed);
        
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
        float rot_speed_abs = fabs(rot_speed);
        if(rot_speed_abs > max_rot_speed) {
            rot_speed = (rot_speed > 0) ? max_rot_speed : -max_rot_speed;
            Ent_SET(rot_speed, ent, rot_speed);
        }
    }

    Ent_SET(rotation, ent, angle_normalize(rotation + rot_speed));
}


void
Ent_m_think(Ent *ent)
{
    float scale = Ent_GET(think_interval, ent) / 1000.0f;
    Ent_update_position(ent, scale);
    Ent_update_rotation(ent, scale);
}

void
Ent_m_touch(Ent *ent, Ent *ent2)
{
}

void
Ent_m_on_frame(Ent *ent)
{
}