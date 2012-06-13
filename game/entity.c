#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL.h>

#include "vec2.h"
#include "entity.h"

const ent_class_t ENT_CLASS_BASE = {1, "GenericEntity", sizeof(ent_t),
	ent_spawn, ent_remove, ent_think, ent_touch, ent_blocked
};

void ent_init(ent_t *ent)
{
	if(ent == NULL)
		return;

	ent->flags = 0;
	ent->position = VEC2_ZERO;
	ent->velocity = VEC2_ZERO;
	ent->speed = 0.0;
	ent->max_speed = -1.0;
	
	ent->rotation = 0.0;
	ent->rot_speed = 0.0;
	ent->max_rot_speed = 360.0;

	ent->prev_think = 0;
	ent->next_think = 0;

	ent->image = NULL;
	ent->ent_class = &ENT_CLASS_BASE;
}

void ent_deinit(ent_t *ent)
{
	return;
}

ent_t *ent_create(const ent_class_t *ent_class)
{
	ent_t *ent;

	if(ent_class == NULL)
		return NULL;

	ent = malloc(ent_class->data_size);
	if(ent == NULL)
		return NULL;

	ent_init(ent);
	return ent;
}

void ent_free(ent_t *ent)
{
	if(ent != NULL) {
		ent_deinit(ent);
		free(ent);
	}
}

void ent_spawn(ent_t *ent)
{
	printf("Ent spawning: %p\n", ent);
}

void ent_remove(ent_t *ent)
{
	printf("Ent removing: %p\n", ent);
}

void ent_think(ent_t *ent)
{
	printf("Ent thinking: %p\n", ent);
}

void ent_touch(ent_t *ent, ent_t *ent2)
{
	printf("Ent touching: %p, %p\n", ent, ent2);
}

void ent_blocked(ent_t *ent, ent_t *ent2)
{
	printf("Ent blocked: %p, %p\n", ent, ent2);
}
