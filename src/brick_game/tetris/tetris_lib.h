#ifndef TETRIS_LIB_H
#define TETRIS_LIB_H

#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>

#include "defines.h"

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef enum {
  Begin,
  Spawn,
  Moving,
  Shifting,
  Attaching,
  GameOver,
  Break
} State;

typedef struct {
  int start_pos[2];
  int blocks_pos[4][2];
  int code_rotate;
} Figure;

typedef struct {
  GameInfo_t game_info;
  State state;
  UserAction_t user_action;
  Figure figure;
  int button_pressed;
  unsigned long long time;
} FullInfo;

FullInfo *get_current_state();
void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();
void sigact();

#endif  // TETRIS_LIB_H
