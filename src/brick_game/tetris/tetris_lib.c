#include "tetris_lib.h"

/**
 * @brief Функция хранения текущего состоянии игры
 * @return Возвращает указатель на объект структуры FullInfo
 */
FullInfo *get_current_state() {
  static FullInfo full_info = {0};
  return &full_info;
}
/**
 * @brief Функция сохранения пользовательского ввода в структуре FullInfo
 * @param action ввод пользователя
 * @param hold флаг обозначающий нажатие
 */
void userInput(UserAction_t action, bool hold) {
  // (void)hold;
  FullInfo *full_info = get_current_state();
  full_info->button_pressed = hold;
  full_info->user_action = action;
}
/**
 * @brief Функция получения текущего состояния игры
 * @return Возвращает ифнормацию о игре в виде структуры GameInfo_t
 */
GameInfo_t updateCurrentState() {
  FullInfo *full_info = get_current_state();

  return full_info->game_info;
}
