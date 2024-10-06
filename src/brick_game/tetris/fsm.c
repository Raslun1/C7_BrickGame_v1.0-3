#include "fsm.h"

/**
 * @brief Конечный автомат
 *
 * Функция обработки текущего состояния игры;<br>
 * Меняет состояние игры в зависимости от ввода пользователя
 */
void sigact() {
  FullInfo *full_info = get_current_state();
  switch (full_info->state) {
    case Begin:
      start_handler(full_info);
      break;
    case Spawn:
      spawn_figure(full_info);
      break;
    case Break:
      pause_handler(full_info);
      break;
    case Moving:
      moving_handler(full_info);
      break;
    case Shifting:
      move_down(full_info);
      break;
    case Attaching:
      attaching_handler(full_info);
      break;
    case GameOver:
      full_info->game_info.pause = 2;
      end_game(full_info);
      full_info->state = Begin;
      break;
    default:
      break;
  }
  full_info->button_pressed = false;
}

/**
 * @brief Функция отвечающая за состояние Начала игры
 *
 * В зависимости от ввода пользователя переходит в состояние Спавна или
 * Выхода из игры
 */
void start_handler(FullInfo *full_info) {
  if (full_info->button_pressed) {
    switch (full_info->user_action) {
      case Start:
        start_game(full_info);
        full_info->state = Spawn;
        break;
      case Terminate:
        full_info->game_info.pause = 3;
        end_game(full_info);
        break;
      default:
        break;
    }
  }
}
/**
 * @brief Функция отвечающая за состояние Спавна фигуры
 *
 * Инициализируются начальные координаты фигуры;<br>
 * Присваиваются координаты следующей фигуры текущей;<br>
 * Создается новая следующая фигура
 */
void spawn_figure(FullInfo *full_info) {
  full_info->figure.start_pos[0] = 0;
  full_info->figure.start_pos[1] = 4;
  for (int i = 0; i < 4; i++) {
    full_info->figure.blocks_pos[i][0] = full_info->game_info.next[i][0];
    full_info->figure.blocks_pos[i][1] = full_info->game_info.next[i][1];
  }
  full_info->figure.code_rotate = full_info->game_info.next[4][0];
  create_next_figure(full_info);
  full_info->state = Moving;
}
/**
 * @brief Функция отвечающая за состояние Движения
 *
 * В зависимости от ввода пользователя двигает фигуру в нужном направлении;<br>
 * В случае истечения времени переходит в состояние Смещения
 */
void moving_handler(FullInfo *full_info) {
  if (full_info->button_pressed) {
    switch (full_info->user_action) {
      case Left:
        move_left(full_info);
        break;
      case Right:
        move_right(full_info);
        break;
      case Down:
        move_down(full_info);
        break;
      case Up:
        break;
      case Action:
        rotate(full_info);
        break;
      case Pause:
        full_info->game_info.pause = 1;
        full_info->state = Break;
        break;
      default:
        break;
    }
  }
  if (check_shifting(full_info)) {
    full_info->state = Shifting;
  }
}
/**
 * @brief Функция отвечающая за состояние Паузы
 *
 * В зависимости от ввода пользователя переходит в состояние Движения или
 * Выхода из игры
 */
void pause_handler(FullInfo *full_info) {
  if (full_info->button_pressed) {
    switch (full_info->user_action) {
      case Pause:
        full_info->game_info.pause = 0;
        full_info->state = Moving;
        break;
      case Terminate:
        full_info->game_info.pause = 3;
        end_game(full_info);
        break;
      default:
        break;
    }
  }
}
/**
 * @brief Функция отвечающая за состояние Присоединение фигуры к полю
 *
 * Происходит обновление поля;<br>
 * Идет проверка на наличие полных линий;<br>
 * В зависимости от заполненности поля переходит в состояние Конца игры или
 * Спавна новой фигуры
 */
void attaching_handler(FullInfo *full_info) {
  refresh_field(full_info);
  check_full_line(full_info);
  if (check_gameover(full_info)) {
    if (full_info->game_info.score > full_info->game_info.high_score) {
      set_highscore(full_info);
    }
    full_info->state = GameOver;
  } else
    full_info->state = Spawn;
}
