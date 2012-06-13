#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#include "entity.h"

int main(int argc, char **argv)
{
	ent_t *entity = ent_create(&ENT_CLASS_BASE);
	(*entity->ent_class->func_spawn)(entity);

	printf("Hello, World!\n");
	return 0;
}