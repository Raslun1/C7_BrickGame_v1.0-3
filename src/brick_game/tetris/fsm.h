#ifndef FSM_H
#define FSM_H

#include "backend.h"
#include "defines.h"
#include "tetris_lib.h"

void sigact();
void start_handler(FullInfo *full_info);
void moving_handler(FullInfo *full_info);
void pause_handler(FullInfo *full_info);
void spawn_figure(FullInfo *full_info);
void attaching_handler(FullInfo *full_info);

#endif  // FSM_H