#ifndef ENTITY_H
#define ENTITY_H

#include <stddef.h>

#include <SDL.h>

#include "vec2.h"

#ifdef _MSC_VER
#define inline __inline
#endif

struct ent_t;

typedef struct {
	unsigned int id;
	const char *name;
	size_t data_size;

	void (*func_spawn)  (struct ent_t*);
	void (*func_remove) (struct ent_t*);
	void (*func_think)  (struct ent_t*);
	void (*func_touch)  (struct ent_t*, struct ent_t*);
	void (*func_blocked)(struct ent_t*, struct ent_t*);
} ent_class_t;

const ent_class_t ENT_CLASS_BASE;

typedef struct ent_t {
	unsigned int flags;

	vec2 position;
	vec2 velocity;
	float speed;
	float max_speed;

	float rotation;
	float rot_speed;
	float max_rot_speed;

	Uint32 prev_think;
	Uint32 next_think;

	SDL_Surface *image;

	const ent_class_t *ent_class;
} ent_t;

void ent_init(ent_t *ent);
void ent_deinit(ent_t *ent);
ent_t *ent_create(const ent_class_t *ent_class);
void ent_free(ent_t *ent);

void ent_spawn(ent_t *ent);
void ent_remove(ent_t *ent);
void ent_think(ent_t *ent);
void ent_touch(ent_t *ent, ent_t *ent2);
void ent_blocked(ent_t *ent, ent_t *ent2);

#define ENT_GETTER_SETTER(type, member) \
	static inline type \
	ent_##member(ent_t *ent) { return ent->member; }; \
	static inline void \
	ent_set_##member(ent_t *ent, type val) { ent->member = val; }

#define ENT_GETTER_SETTER_PTR(type, member) \
	static inline const type * \
	ent_##member(ent_t *ent) { return &ent->member; }; \
	static inline void \
	ent_set_##member(ent_t *ent, const type *val) { ent->member = *val; }

ENT_GETTER_SETTER(Uint32, flags);

ENT_GETTER_SETTER_PTR(vec2, position);

ENT_GETTER_SETTER_PTR(vec2, velocity);
void ent_set_vel_dir(ent_t *ent, vec2 *dir);
ENT_GETTER_SETTER(float, speed);
ENT_GETTER_SETTER(float, max_speed);

ENT_GETTER_SETTER(float, rotation);
void ent_rotate(ent_t *ent, float rotation);
ENT_GETTER_SETTER(float, rot_speed);
ENT_GETTER_SETTER(float, max_rot_speed);

#endif
