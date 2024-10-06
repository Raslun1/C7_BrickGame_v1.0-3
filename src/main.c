#include "main.h"

#include <time.h>
void run();

/**
 * @brief Основная функция
 *
 * Инициализируется окно терминала ncurses и задаются его настройки;<br>
 * Инициализируется генератор случайных чисел;<br>
 * Запускается игра;<br>
 * По окончании игры очищается память выделенная под окно ncurses
 */
int main() {
  WINDOW_INIT(50);
  setlocale(LC_ALL, "");
  srand(time(0));
  run();
  endwin();
}
/**
 * @brief Функция отвечающая за считывание пользовательского ввода
 */
void get_user_input(int user_action) {
  switch (user_action) {
    case 'e':
      userInput(Start, true);
      break;
    case 'q':
      userInput(Terminate, true);
      break;
    case KEY_LEFT:
      userInput(Left, true);
      break;
    case KEY_RIGHT:
      userInput(Right, true);
      break;
    case KEY_DOWN:
      userInput(Down, true);
      break;
    case KEY_UP:
      break;
    case SPACE:
      userInput(Action, true);
      break;
    case ESCAPE:
      userInput(Pause, true);
      break;
    default:
      break;
  }
}

/**
 * @brief Функция соприкосновения библиотеки игры и графического интерфейса
 *
 * Получается вся информация о текущем состоянии игры;<br>
 * Вызывается основная функцию отрисовки;<br>
 * Вызывается конечный автомат;<br>
 * В конце очищается консоль
 */
void run() {
  bool flag = true;

  while (flag) {
    GameInfo_t game_info = updateCurrentState();
    fill_window(game_info);
    get_user_input(INPUT);
    sigact();
    if (game_info.pause == 3) flag = false;

    refresh();
    clear();
  }
}
