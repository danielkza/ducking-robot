#include <stdlib.h>
#include <stdio.h>
#include <time.h>


// Falta implementar a função remove_bomb()


typedef struct {
    float x, y;
} vec2;

typedef struct {
    vec2 wind_velocity;
    vec2 position;
    vec2 velocity;
    float weight;
} boat_params;

typedef struct {
    float modifier;
    float remaining_update_time;
    vec2 position;
} bomb_params;

#define checked_fscanf()

int read_vec2(FILE *in, vec2 *vec)
{
    float x, y;
    if(fscanf(in, "%f %f", &x, &y) != 2)
        return 0;

    vec->x = x;
    vec->y = y;

    return 1;
}

void free_data(boat_params *boat, bomb_params **bombs, int bomb_count)
{
    free(boat);
    if(bombs != NULL) {
        int i;
        for(i = 0; i < bomb_count; i++) {
            free(bombs[i]);
        }
        free(bombs);
    }
}

int read_input(FILE *in, boat_params **boat_out, bomb_params ***bombs_out, int *bomb_count_out)
{
    boat_params *boat = NULL;
    bomb_params **bombs = NULL;
    int bomb_count;

    boat = malloc(sizeof(*boat));
    if(boat == NULL)
        goto error;

    fscanf(in, " .w");
    if(!read_vec2(in, &(boat->wind_velocity)))
        goto error;

    if(fscanf(in, " .b %d", &bomb_count) != 1 || bomb_count <= 0)
        goto error;

    bombs = calloc(bomb_count, sizeof(*bombs));
    if(bombs == NULL)
        goto error;

    int i;
    for(i = 0; i < bomb_count; i++) {
        bombs[i] = malloc(sizeof(bomb_params));
        if(bombs[i] == NULL)
            goto error;

        if(fscanf(in, "%f %f", &(bombs[i]->modifier),
                               &(bombs[i]->remaining_update_time)) != 2)
        {
            goto error;
        }

        if(!read_vec2(in, &(bombs[i]->position))) {
            goto error;
        }
    }

    if(fscanf(in, " .s %f", &(boat->weight)) != 1)
        goto error;

    if(!read_vec2(in, &(boat->position))
        || !read_vec2(in, &(boat->velocity)))
    {
        goto error;
    }

    *boat_out = boat;
    *bombs_out = bombs;
    *bomb_count_out = bomb_count;

    return 1;

error:
    free_data(boat, bombs, bomb_count);
    fprintf(stderr, "Parsing error\n");

    return 0;
}


int print_output(FILE *out, boat_params *boat, bomb_params **bombs, int bomb_count, int total_bombs)
{
    fprintf(out, ".w\n");
    fprintf(out, "%f %f\n", boat->wind_velocity.x, boat->wind_velocity.y);

    fprintf(out, ".b %d\n", bomb_count);

    int i;
    for(i = 0; i < total_bombs; i++) {
        if(bombs[i]->remaining_update_time <= 0){
            printf("\n");
            continue;
        }
        fprintf(out, "%f %f %f %f\n", bombs[i]->modifier, bombs[i]->remaining_update_time,
                bombs[i]->position.x, bombs[i]->position.y);
    }

    fprintf(out, ".s\n");
    fprintf(out, "%f\n", boat->weight);
    fprintf(out, "%f %f\n", boat->position.x, boat->position.y);
    fprintf(out, "%f %f\n", boat->velocity.x, boat->velocity.y);
    fprintf(out, "\n");
}

int interact(boat_params *boat, bomb_params **bombs, int *bomb_count, int total_bombs, float update_time)
{
    int i;
    vec2 boat_acel;
    boat_acel.x = boat->wind_velocity.x / boat->weight;
    boat_acel.y = boat->wind_velocity.y / boat->weight;

    boat->position.x += boat->velocity.x * update_time + (boat_acel.x * update_time * update_time) / 2;
    boat->position.y += boat->velocity.y * update_time + (boat_acel.y * update_time * update_time) / 2;

    boat->velocity.x += boat_acel.x * update_time;
    boat->velocity.y += boat_acel.y * update_time;

    for(i = 0; i < total_bombs; i++) {
        if(bombs[i]->remaining_update_time <= 0)
            continue;
        bombs[i]->remaining_update_time -= update_time;
        if(bombs[i]->remaining_update_time <= 0){
            (*bomb_count)--;
        }else if(bombs[i]->position.x == boat->position.x && bombs[i]->position.y == boat->position.y){
            (*bomb_count)--;
        }
    }
}

void wait (float seconds)
{
  clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {}
}

int main(int argc, char**argv)
{
    int i;
    boat_params *boat;
    bomb_params **bombs;
    int bomb_count;
    int total_bombs;
    float update_time;
    float simulation_time;

    if(argc != 3){
        printf("Missing arguments\n");
        return 0;
    }

    update_time = atof(argv[1]);
    simulation_time = atof(argv[2]);

    if(!read_input(stdin, &boat, &bombs, &bomb_count))
        exit(EXIT_FAILURE);

    total_bombs = bomb_count;

    print_output(stdout, boat, bombs, bomb_count, total_bombs);

    // Loop Principal
    while(simulation_time>0)
    {
        interact(boat, bombs, &bomb_count, total_bombs, update_time);
        wait(update_time);
        print_output(stdout, boat, bombs, bomb_count, total_bombs);
        simulation_time -= update_time;
    }

    free_data(boat, bombs, bomb_count);
    return 0;
}