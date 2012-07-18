/**
  * @file
  * Entity class system and utilities
  **/

#ifndef ENT_BASE_H
#define ENT_BASE_H

typedef struct Ent Ent;

/** Datatype representing the class of a game entity **/
typedef struct ent_class_t {
    /** Entity name (arbitrary) **/
    const char *name;
    /** Size of memory block, in bytes, to be allocated for the entity struct **/
    size_t data_size;
    /** Pointer to base class **/
    const struct ent_class_t *base;
    /** Initialization function **/
    void (*m_init)(Ent *ent);
    /** Finalization function **/
    void (*m_destroy)(Ent *ent);
} ent_class_t;

/**
 * Checks if a class is a subclass of some other.
 *
 * @param test Class to test
 * @param base Base class
 * 
 * @returns 1 if test is a a subclass of base, 0 otherwise
 **/

int
ent_class_is_subclass(const ent_class_t *test, const ent_class_t *base);

/**
 * Allocates memory for an entity of a class and initializes it, calling the
 * constructors for all it's base classes.
 * 
 * @param eclass Pointer to entity class
 *
 * @returns Created entity
 **/
Ent
*Ent_m_create(const ent_class_t *eclass);

/**
 * Convenience macro for creating entities.
 *
 * @param eclass Class name
 *
 * @returns Created entity
 *
 * Example:
 * @code Ent *ent = ENT_CREATE(VisibleEnt); @endcode
 **/
#define ENT_CREATE(eclass) \
    ((eclass*)Ent_m_create(&eclass##_CLASS))

/**
 * Destroys an entity, calling all its destructors and deallocating its memory.
 *
 * @param ent Entity to free
 **/
void
Ent_m_free(Ent *ent);

/**
 * Convenience macro for destroying entities.
 *
 * @param ent Entity to free
 **/

#define ENT_FREE(ent) \
    Ent_m_free((Ent*)(ent))

#endif