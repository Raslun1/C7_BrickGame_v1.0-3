#include <check.h>

#include "brick_game/tetris/backend.h"
#include "brick_game/tetris/fsm.h"
#include "brick_game/tetris/tetris_lib.h"

START_TEST(test_tetris_01_alloc_dealloc_memory) {
  FullInfo full_info = {0};
  allocate_mem(&full_info.game_info.field, HEIGHT, WIDTH);
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      ck_assert_int_eq(0, full_info.game_info.field[i][j]);
    }
  }
  free_mem(&full_info.game_info.field, HEIGHT);
}
END_TEST

START_TEST(test_tetris_02_start_game) {
  FullInfo full_info = {0};
  start_game(&full_info);
  ck_assert_int_eq(0, full_info.game_info.pause);
  ck_assert_int_eq(0, full_info.game_info.score);
  ck_assert_int_eq(1, full_info.game_info.level);
  free_mem(&full_info.game_info.field, HEIGHT);
  free_mem(&full_info.game_info.next, 5);
}
END_TEST

START_TEST(test_tetris_03_spawn_figure) {
  FullInfo full_info = {0};
  start_game(&full_info);
  spawn_figure(&full_info);

  ck_assert_int_eq(0, full_info.figure.start_pos[0]);
  ck_assert_int_eq(4, full_info.figure.start_pos[1]);

  free_mem(&full_info.game_info.field, HEIGHT);
  free_mem(&full_info.game_info.next, 5);
}
END_TEST

START_TEST(test_tetris_04_move_left_rigth) {
  FullInfo full_info = {0};
  start_game(&full_info);
  spawn_figure(&full_info);

  move_left(&full_info);

  ck_assert_int_eq(0, full_info.figure.start_pos[0]);
  ck_assert_int_eq(4 - 1, full_info.figure.start_pos[1]);

  move_right(&full_info);

  ck_assert_int_eq(0, full_info.figure.start_pos[0]);
  ck_assert_int_eq(4, full_info.figure.start_pos[1]);

  free_mem(&full_info.game_info.field, HEIGHT);
  free_mem(&full_info.game_info.next, 5);
}
END_TEST

START_TEST(test_tetris_05_check_collision) {
  FullInfo full_info = {0};
  start_game(&full_info);
  spawn_figure(&full_info);

  if (!check_collision(full_info.game_info, full_info.figure)) {
    full_info.figure.start_pos[0]++;
  }
  ck_assert_int_eq(0 + 1, full_info.figure.start_pos[0]);
  ck_assert_int_eq(4, full_info.figure.start_pos[1]);

  for (int i = 0; i < WIDTH; i++) {
    full_info.game_info.field[0 + 2][i] = 1;  // устанавливаем препятствие
  }
  if (!check_collision(full_info.game_info, full_info.figure)) {
    full_info.figure.start_pos[0]++;
  }

  ck_assert_int_eq(0 + 1, full_info.figure.start_pos[0]);
  ck_assert_int_eq(4, full_info.figure.start_pos[1]);

  free_mem(&full_info.game_info.field, HEIGHT);
  free_mem(&full_info.game_info.next, 5);
}
END_TEST

START_TEST(test_tetris_06_move_down) {
  FullInfo full_info = {0};
  start_game(&full_info);
  spawn_figure(&full_info);

  while (!check_collision(full_info.game_info, full_info.figure))
    move_down(&full_info);

  ck_assert_int_eq(true, full_info.figure.start_pos[0] < HEIGHT);
  ck_assert_int_eq(4, full_info.figure.start_pos[1]);

  free_mem(&full_info.game_info.field, HEIGHT);
  free_mem(&full_info.game_info.next, 5);
}
END_TEST

START_TEST(test_tetris_07_action) {
  FullInfo full_info = {0};
  start_game(&full_info);
  spawn_figure(&full_info);

  Figure fig = {0};
  fig.start_pos[0] = full_info.figure.start_pos[0];
  fig.start_pos[1] = full_info.figure.start_pos[1];

  for (int i = 0; i < 4; i++) {
    rotate(&full_info);
  }

  ck_assert_int_eq(fig.start_pos[0], full_info.figure.start_pos[0]);
  ck_assert_int_eq(fig.start_pos[1], full_info.figure.start_pos[1]);

  free_mem(&full_info.game_info.field, HEIGHT);
  free_mem(&full_info.game_info.next, 5);
}
END_TEST

START_TEST(test_tetris_08_action_next_to_the_wall) {
  FullInfo full_info = {0};
  start_game(&full_info);
  spawn_figure(&full_info);

  for (int i = 0; i < 10; i++) {
    move_left(&full_info);
    rotate(&full_info);
  }
  ck_assert_int_eq(true, full_info.figure.start_pos[1] > -1);

  for (int i = 0; i < 10; i++) {
    move_right(&full_info);
    rotate(&full_info);
  }
  ck_assert_int_eq(true, full_info.figure.start_pos[1] < WIDTH);

  free_mem(&full_info.game_info.field, HEIGHT);
  free_mem(&full_info.game_info.next, 5);
}
END_TEST

START_TEST(test_tetris_09_cleanField_refreshField_recordScore) {
  FullInfo full_info = {0};
  start_game(&full_info);
  spawn_figure(&full_info);

  int scores[4] = {100, 300, 700, 1500};

  for (int i = 1; i < 5; i++) {
    for (int j = 1; j < i + 1; j++) {
      for (int k = 0; k < WIDTH; k++) {
        full_info.game_info.field[j][k] = 1;
      }
    }
    check_full_line(&full_info);
    ck_assert_int_eq(full_info.game_info.score, scores[i - 1]);
    full_info.game_info.score = 0;
  }

  refresh_field(&full_info);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(
        1, full_info.game_info.field[full_info.figure.start_pos[0] +
                                     full_info.figure.blocks_pos[i][0]]
                                    [full_info.figure.start_pos[1] +
                                     full_info.figure.blocks_pos[i][1]]);
  }

  free_mem(&full_info.game_info.field, HEIGHT);
  free_mem(&full_info.game_info.next, 5);
}
END_TEST

START_TEST(test_tetris_11_get_set_highScore) {
  FullInfo full_info = {0};

  get_highscore(&full_info);
  int res1 = full_info.game_info.high_score;
  full_info.game_info.score = res1;
  set_highscore(&full_info);
  get_highscore(&full_info);
  int res2 = full_info.game_info.high_score;

  ck_assert_int_eq(res1, res2);

  end_game(&full_info);
}
END_TEST

START_TEST(test_tetris_12_FSM_Start) {
  FullInfo *full_info = get_current_state();
  full_info->state = Begin;
  userInput(Start, true);
  sigact();

  ck_assert_ptr_nonnull(full_info->game_info.field);
  ck_assert_ptr_nonnull(full_info->game_info.next);
  ck_assert_int_eq(Spawn, full_info->state);

  full_info->state = Begin;
  userInput(Terminate, true);
  sigact();
  ck_assert_ptr_null(full_info->game_info.field);
  ck_assert_ptr_null(full_info->game_info.next);
}
END_TEST

START_TEST(test_tetris_13_FSM_Spawn) {
  FullInfo *full_info = get_current_state();
  full_info->state = Begin;
  userInput(Start, true);
  sigact();
  sigact();
  ck_assert_int_eq(Moving, full_info->state);
  ck_assert_int_eq(0, full_info->figure.start_pos[0]);
  ck_assert_int_eq(4, full_info->figure.start_pos[1]);

  full_info->state = Begin;
  userInput(Terminate, true);
  sigact();
}
END_TEST

START_TEST(test_tetris_14_FSM_Moving) {
  FullInfo *full_info = get_current_state();
  full_info->state = Begin;
  userInput(Start, true);
  sigact();
  sigact();
  userInput(Up, true);
  sigact();
  userInput(Left, true);
  sigact();
  ck_assert_int_eq(4 - 1, full_info->figure.start_pos[1]);
  userInput(Right, true);
  sigact();
  ck_assert_int_eq(4, full_info->figure.start_pos[1]);
  userInput(Down, true);
  sigact();
  ck_assert_int_ne(0, full_info->figure.start_pos[0]);
  userInput(Pause, true);
  sigact();
  ck_assert_int_eq(Break, full_info->state);
  full_info->state = Begin;
  userInput(Terminate, true);
  sigact();
}
END_TEST

START_TEST(test_tetris_15_FSM_Break) {
  FullInfo *full_info = get_current_state();
  full_info->state = Begin;
  userInput(Start, true);
  sigact();
  sigact();
  userInput(Pause, true);
  sigact();
  ck_assert_int_eq(Break, full_info->state);
  ck_assert_int_eq(1, full_info->game_info.pause);
  userInput(Pause, true);
  sigact();
  ck_assert_int_eq(0, full_info->game_info.pause);

  full_info->state = Break;
  userInput(Terminate, true);
  sigact();
  ck_assert_ptr_null(full_info->game_info.field);
  ck_assert_ptr_null(full_info->game_info.next);
}
END_TEST

START_TEST(test_tetris_16_FSM_Shifting) {
  FullInfo *full_info = get_current_state();
  full_info->state = Begin;
  userInput(Start, true);
  sigact();
  full_info->state = Shifting;
  ck_assert_int_eq(Shifting, full_info->state);
  sigact();
  ck_assert_int_eq(Moving, full_info->state);

  full_info->state = Break;
  userInput(Terminate, true);
  sigact();
  ck_assert_ptr_null(full_info->game_info.field);
  ck_assert_ptr_null(full_info->game_info.next);
}
END_TEST

START_TEST(test_tetris_17_FSM_Attaching) {
  FullInfo *full_info = get_current_state();
  full_info->state = Begin;
  userInput(Start, true);
  sigact();
  sigact();
  while (!check_collision(full_info->game_info, full_info->figure)) {
    move_down(full_info);
  }
  userInput(Down, true);
  sigact();
  ck_assert_int_eq(Attaching, full_info->state);
  sigact();
  ck_assert_int_eq(Spawn, full_info->state);
  GameInfo_t game = updateCurrentState();
  ck_assert_int_eq(1, game.level);

  full_info->state = Break;
  userInput(Terminate, true);
  sigact();
  ck_assert_ptr_null(full_info->game_info.field);
  ck_assert_ptr_null(full_info->game_info.next);
}
END_TEST

START_TEST(test_tetris_18_FSM_GameOver) {
  FullInfo *full_info = get_current_state();
  full_info->state = Begin;
  userInput(Start, true);
  sigact();
  sigact();

  full_info->state = GameOver;
  sigact();
  ck_assert_int_eq(2, full_info->game_info.pause);
  ck_assert_int_eq(1, full_info->game_info.level);

  userInput(Terminate, true);
  sigact();
  ck_assert_ptr_null(full_info->game_info.field);
  ck_assert_ptr_null(full_info->game_info.next);
}
END_TEST

Suite *s21_tetris_suite(void) {
  Suite *suite;
  suite = suite_create("s21_tetris");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_tetris_01_alloc_dealloc_memory);
  tcase_add_test(tc_core, test_tetris_02_start_game);
  tcase_add_test(tc_core, test_tetris_03_spawn_figure);
  tcase_add_test(tc_core, test_tetris_04_move_left_rigth);
  tcase_add_test(tc_core, test_tetris_05_check_collision);
  tcase_add_test(tc_core, test_tetris_06_move_down);
  tcase_add_test(tc_core, test_tetris_07_action);
  tcase_add_test(tc_core, test_tetris_08_action_next_to_the_wall);
  tcase_add_test(tc_core, test_tetris_09_cleanField_refreshField_recordScore);
  tcase_add_test(tc_core, test_tetris_11_get_set_highScore);

  tcase_add_test(tc_core, test_tetris_12_FSM_Start);
  tcase_add_test(tc_core, test_tetris_13_FSM_Spawn);
  tcase_add_test(tc_core, test_tetris_14_FSM_Moving);
  tcase_add_test(tc_core, test_tetris_15_FSM_Break);
  tcase_add_test(tc_core, test_tetris_16_FSM_Shifting);
  tcase_add_test(tc_core, test_tetris_17_FSM_Attaching);
  tcase_add_test(tc_core, test_tetris_18_FSM_GameOver);

  suite_add_tcase(suite, tc_core);

  return suite;
}

int main() {
  Suite *suite = s21_tetris_suite();
  SRunner *suite_runner = srunner_create(suite);
  srunner_run_all(suite_runner, CK_VERBOSE);
  int failed_count = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
  return (failed_count == 0) ? 0 : 1;
}