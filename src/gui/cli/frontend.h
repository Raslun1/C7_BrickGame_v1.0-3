#ifndef FRONTEND_H
#define FRONTEND_H

#include <ncurses.h>

#include "../../brick_game/tetris/tetris_lib.h"

void fill_window(GameInfo_t game_info);
void print_Figure();
void print_boardGame(GameInfo_t game_info);
void print_boardInfo(GameInfo_t game_info);
void print_borders();
void print_pauseInfo();
void print_gameOver();
void print_startGame();

#endif  // FRONTEND_H
