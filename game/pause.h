#ifndef PAUSE_H
#define PAUSE_H

int is_started();
void toggle_started();
void set_started(int start);

int is_paused();
void toggle_paused();
void set_paused(int pause);

int is_game_over();
void toggle_game_over();
int set_game_over(int game_over);

#endif