#include "pause.h"

static int started = 0, paused = 1, game_over = 0;

int is_started()
{
    return started;
}

void toggle_started()
{
    started = !started;
}


void set_started(int val)
{
    started = (val != 0);
}

int is_paused()
{
    return paused;
}

void toggle_paused()
{
    paused = !paused;
}

void set_paused(int val)
{
    paused = (val != 0);
}

int is_game_over()
{
    return game_over;
}

void toggle_game_over()
{
    game_over = !game_over;
}

int set_game_over(int val)
{
    game_over = (val != 0);
}

