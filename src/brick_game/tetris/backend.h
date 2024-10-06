#ifndef BACKEND_H
#define BACKEND_H

#include <stdio.h>

#include "defines.h"
#include "tetris_lib.h"

void get_highscore(FullInfo *full_info);
void set_highscore(FullInfo *full_info);
void create_next_figure(FullInfo *full_info);
void start_game(FullInfo *full_info);
void end_game(FullInfo *full_info);
void move_left(FullInfo *full_info);
void move_right(FullInfo *full_info);
bool check_collision(GameInfo_t game_info, Figure figure);
void move_down(FullInfo *full_info);
Figure rotated_figure(FullInfo *full_info);
void rotate(FullInfo *full_info);
void refresh_field(FullInfo *full_info);
bool check_gameover(FullInfo *full_info);
void set_level(FullInfo *full_info);
void check_full_line(FullInfo *full_info);
unsigned long long current_time();
bool check_shifting(FullInfo *full_info);
void allocate_mem(int ***matrix, int n, int m);
void free_mem(int ***matrix, int n);

#endif  // BACKEND_H
