#define WINDOW_INIT(time) \
  {                       \
    initscr();            \
    noecho();             \
    curs_set(0);          \
    keypad(stdscr, TRUE); \
    timeout(time);        \
  }
#define WIDTH 10
#define HEIGHT 20
#define CENTER 20

#define MENU 8

#define INPUT getch()

#define ESCAPE 27
#define SPACE 32

#define HIGHSCORE "./brick_game/tetris/high_score.txt"
