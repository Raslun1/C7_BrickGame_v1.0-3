#include "frontend.h"

/**
 * @brief Основная функция отрисовки
 *
 * Отрисовывается границы игрового поля;<br>
 * Отрисовывается дополнительная информация (уровень, кол-во очков и т.д.);<br>
 * В зависимости от состояния отрисовывается начало, конец или пауза;<br>
 * После начала отрисовывается фигура и игровое поле
 */
void fill_window(GameInfo_t game_info) {
  print_borders();
  print_boardInfo(game_info);

  if (game_info.field == NULL) print_startGame();
  if (game_info.pause == 1)
    print_pauseInfo();
  else if (game_info.pause == 2)
    print_gameOver();
  else {
    if (game_info.field != NULL) {
      print_boardGame(game_info);
    }
    if (game_info.next != NULL) {
      print_Figure();
    }
  }
}
/**
 * @brief Отрисовка фигуры
 */
void print_Figure() {
  FullInfo *full_info = get_current_state();
  Figure figure = full_info->figure;
  if (figure.code_rotate != 0) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 2; j++) {
        mvaddch(figure.start_pos[0] + figure.blocks_pos[i][0] + 1,
                CENTER + (figure.start_pos[1] + figure.blocks_pos[i][1]) * 2 +
                    j + 2,
                ACS_CKBOARD);
      }
    }
  }
}
/**
 * @brief Отрисовка игрового поля
 */
void print_boardGame(GameInfo_t game_info) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      for (int k = 0; k < 2; k++) {
        if (game_info.field[i][j] == 1) {
          mvaddch(i + 1, CENTER + j * 2 + k + 2, ACS_CKBOARD);
        }
      }
    }
  }
}
/**
 * @brief Отрисовка дополнительной информации
 */
void print_boardInfo(GameInfo_t game_info) {
  int x = CENTER + 2 * WIDTH + 3;
  if (game_info.high_score < game_info.score) {
    mvprintw(1, x, "High %d", game_info.score);
  } else {
    mvprintw(1, x, "High %d", game_info.high_score);
  }
  mvprintw(3, x, "Score %d", game_info.score);
  mvprintw(5, x, "Level %d", game_info.level);
  mvprintw(10, x, "Next");
  if (game_info.next != NULL) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 2; j++) {
        mvaddch(12 + game_info.next[i][0], x + 2 + game_info.next[i][1] * 2 + j,
                ACS_CKBOARD);
      }
    }
  }
}
/**
 * @brief Отрисовка границ игрового поля
 */
void print_borders() {
  mvaddch(0, CENTER + 1, ACS_ULCORNER);
  mvhline(0, CENTER + 2, ACS_HLINE, WIDTH * 3 + 2);
  mvaddch(0, CENTER + WIDTH * 3 + 3, ACS_URCORNER);

  mvvline(1, CENTER + 1, ACS_VLINE, HEIGHT);
  mvvline(1, CENTER + WIDTH * 2 + 2, ACS_VLINE, HEIGHT);
  mvvline(1, CENTER + WIDTH * 3 + 3, ACS_VLINE, HEIGHT);

  mvaddch(HEIGHT + 1, CENTER + 1, ACS_LLCORNER);
  mvhline(HEIGHT + 1, CENTER + 2, ACS_HLINE, WIDTH * 3 + 2);
  mvaddch(HEIGHT + 1, CENTER + WIDTH * 3 + 3, ACS_LRCORNER);

  mvaddch(0, CENTER + WIDTH * 2 + 2, ACS_TTEE);
  mvaddch(HEIGHT + 1, CENTER + WIDTH * 2 + 2, ACS_BTEE);

  refresh();
}
/**
 * @brief Отрисовка паузы
 */
void print_pauseInfo() {
  mvprintw(HEIGHT / 2 - 2, CENTER + 7, "GAME PAUSED");
  mvprintw(HEIGHT / 2, CENTER + 7, "Esc - continue");
  mvprintw(HEIGHT / 2 + 1, CENTER + 7, "q - exit");
}
/**
 * @brief Отрисовка информации об окончании игры
 */
void print_gameOver() { mvprintw(HEIGHT / 2 - 2, CENTER + 7, "GAME OVER"); }
/**
 * @brief Отрисовка начальной информации
 */
void print_startGame() {
  mvprintw(HEIGHT / 2, CENTER + 7, "e - new game");
  mvprintw(HEIGHT / 2 + 1, CENTER + 7, "q - exit");
}
