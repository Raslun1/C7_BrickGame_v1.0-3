#include "backend.h"

/**
 * @brief Функция считывания рекорда с файла
 */
void get_highscore(FullInfo *full_info) {
  FILE *fp = fopen(HIGHSCORE, "r");
  if (fscanf(fp, "%d", &full_info->game_info.high_score) != 1) {
    full_info->game_info.high_score = 0;
  }
  fclose(fp);
}
/**
 * @brief Функция записи рекорда в файл
 */
void set_highscore(FullInfo *full_info) {
  FILE *fp = fopen(HIGHSCORE, "w");
  fprintf(fp, "%d", full_info->game_info.score);
  fclose(fp);
}

/**
 * @brief Функция создания новой фигуры
 *
 * Содержит шаблоны фигур и случайным образом создает новую фигуру
 */
void create_next_figure(FullInfo *full_info) {
  int blocks_pos_templates[7][4][2] = {{{0, 0}, {1, 0}, {0, 1}, {1, 1}},
                                       {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
                                       {
                                           {0, 2},
                                           {1, 0},
                                           {1, 1},
                                           {1, 2},
                                       },
                                       {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
                                       {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
                                       {{0, 1}, {1, 0}, {1, 1}, {1, 2}},
                                       {{1, 0}, {1, 1}, {1, 2}, {1, 3}}};

  int code_rotate_templates[] = {
      2, 3, 3, 3, 3, 3, 4,
  };
  int template_ind = rand() % 7;
  for (int i = 0; i < 4; i++) {
    full_info->game_info.next[i][0] = blocks_pos_templates[template_ind][i][0];
    full_info->game_info.next[i][1] = blocks_pos_templates[template_ind][i][1];
  }
  full_info->game_info.next[4][0] = code_rotate_templates[template_ind];
}

/**
 * @brief Функция отвечающая за начало игры
 *
 * Происходит выделение памяти под игровое поле и фигуру;<br>
 * Инициализируется дополнительная информация об игре
 */
void start_game(FullInfo *full_info) {
  allocate_mem(&full_info->game_info.field, HEIGHT, WIDTH);
  allocate_mem(&full_info->game_info.next, 5, 2);
  full_info->game_info.pause = 0;
  full_info->game_info.score = 0;
  full_info->game_info.level = 1;
  full_info->game_info.speed = 1;
  get_highscore(full_info);
  create_next_figure(full_info);
}
/**
 * @brief Функция отвечающая за выход из игры
 *
 * Происходит очищение памяти игрового поля и фигуры
 */
void end_game(FullInfo *full_info) {
  if (full_info->game_info.field != NULL) {
    free_mem(&full_info->game_info.field, HEIGHT);
    free_mem(&full_info->game_info.next, 5);
  }
}
/**
 * @brief Функция отвечающая за движение влево
 */
void move_left(FullInfo *full_info) {
  Figure figure = full_info->figure;
  bool flag = true;
  for (int i = 0; i < 4; i++) {
    if (figure.start_pos[1] + figure.blocks_pos[i][1] - 1 < 0 ||
        (full_info->game_info
             .field[figure.start_pos[0] + figure.blocks_pos[i][0]]
                   [figure.start_pos[1] + figure.blocks_pos[i][1] - 1] == 1))
      flag = false;
  }
  if (flag) full_info->figure.start_pos[1] -= 1;
}
/**
 * @brief Функция отвечающая за движение вправо
 */
void move_right(FullInfo *full_info) {
  Figure figure = full_info->figure;
  bool flag = true;
  for (int i = 0; i < 4; i++) {
    if ((figure.start_pos[1] + figure.blocks_pos[i][1] + 2) > WIDTH ||
        (full_info->game_info
             .field[figure.start_pos[0] + figure.blocks_pos[i][0]]
                   [figure.start_pos[1] + figure.blocks_pos[i][1] + 1] == 1))
      flag = false;
  }
  if (flag) full_info->figure.start_pos[1] += 1;
}
/**
 * @brief Функция проверки столкновения с игровым полем
 * @return Возвращает флаг true если фигура столкуналсь с границей или
 * игровым полем
 */
bool check_collision(GameInfo_t game_info, Figure figure) {
  bool flag = false;
  for (int i = 0; i < 4; i++) {
    if ((figure.blocks_pos[i][0] + figure.start_pos[0] >= HEIGHT - 1) ||
        game_info.field[figure.blocks_pos[i][0] + figure.start_pos[0] + 1]
                       [figure.blocks_pos[i][1] + figure.start_pos[1]] == 1)
      flag = true;
  }
  return flag;
}
/**
 * @brief Функция отвечающая за движение вниз
 *
 * В случае столкновения переходит в состояние Присоединения,
 * иначе состояние Движения
 */
void move_down(FullInfo *full_info) {
  if (!check_collision(full_info->game_info, full_info->figure)) {
    full_info->figure.start_pos[0]++;
    full_info->state = Moving;
  } else
    full_info->state = Attaching;
}
/**
 * @brief Функция отвечающая за поворот фигуры
 * @return Возвращает новую повернутую на 90 градусов фигуру
 */
Figure rotated_figure(FullInfo *full_info) {
  Figure figure = full_info->figure;
  int N = figure.code_rotate;
  int **matrix = NULL;
  int **result_matrix = NULL;
  allocate_mem(&matrix, N, N);
  allocate_mem(&result_matrix, N, N);

  for (int i = 0; i < 4; i++) {
    matrix[figure.blocks_pos[i][0]][figure.blocks_pos[i][1]] = 1;
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      result_matrix[j][N - i - 1] = matrix[i][j];
    }
  }

  int k = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (result_matrix[i][j]) {
        figure.blocks_pos[k][0] = i;
        figure.blocks_pos[k][1] = j;
        k++;
      }
    }
  }

  free_mem(&matrix, N);
  free_mem(&result_matrix, N);
  return figure;
}
/**
 * @brief Функция отвечающая за обработку поворота
 *
 * Сначала создается новая повернутая фигура;<br>
 * Затем проверяются координаты этой фигуры по горизонтали,
 * в случае выхода фигуры за границы она смещается во внутрь;<br>
 * Затем проверяются координаты вокруг фигуры. Если фиура не сталкивается с
 * полем, то новая фигура присваивается текущей.
 */
void rotate(FullInfo *full_info) {
  Figure new_figure = rotated_figure(full_info);
  while (new_figure.start_pos[1] < 0) new_figure.start_pos[1]++;
  while (new_figure.start_pos[1] + new_figure.code_rotate > WIDTH)
    new_figure.start_pos[1]--;

  if (!check_collision(full_info->game_info, new_figure)) {
    full_info->figure = new_figure;
    full_info->figure.start_pos[1] = new_figure.start_pos[1];
  }
}
/**
 * @brief Функция отвечающая за обновление поля в случае столкновения
 */
void refresh_field(FullInfo *full_info) {
  for (int i = 0; i < 4; i++) {
    full_info->game_info.field[full_info->figure.start_pos[0] +
                               full_info->figure.blocks_pos[i][0]]
                              [full_info->figure.start_pos[1] +
                               full_info->figure.blocks_pos[i][1]] = 1;
  }
}
/**
 * @brief Функция проверки конца игры
 */
bool check_gameover(FullInfo *full_info) {
  for (int i = 0; i < WIDTH; i++) {
    if (full_info->game_info.field[1][i]) return true;
  }
  return false;
}
/**
 * @brief Функция установки нового уровня
 */
void set_level(FullInfo *full_info) {
  int lvl = full_info->game_info.level;
  int score = full_info->game_info.score;
  if (lvl < 10 && (score > (lvl * 600))) full_info->game_info.level++;
}
/**
 * @brief Функция проверки полных линий
 *
 * Функция проходит по всем строкам игрового поля и проверяет,
 * является ли каждая из них полной. Если линия полная,
 * она удаляется, а все блоки выше опускаются на одну позицию.
 * Затем обновляется счет игры и уровень сложности.
 */
void check_full_line(FullInfo *full_info) {
  int lines_cleared = 0;
  int score_per_line[5] = {0, 100, 300, 700, 1500};

  for (int i = 0; i < HEIGHT; i++) {
    int count = 0;
    for (int j = 0; j < WIDTH; j++) {
      if (full_info->game_info.field[i][j] == 1) count++;
    }
    if (count == WIDTH) {
      lines_cleared++;
      for (int k = i; k > 0; k--) {
        for (int l = 0; l < WIDTH; l++) {
          full_info->game_info.field[k][l] =
              full_info->game_info.field[k - 1][l];
        }
      }

      for (int l = 0; l < WIDTH; l++) {
        full_info->game_info.field[0][l] = 0;
      }
    }
  }

  if (lines_cleared > 0) {
    full_info->game_info.score += score_per_line[lines_cleared];
  }
  set_level(full_info);
}
/**
 * @brief Функция получения текущего времени
 * @return Возвращает текущее время в миллисекундах
 */
unsigned long long current_time() {
  struct timeval time;
  gettimeofday(&time, NULL);
  return (unsigned long long)(time.tv_sec) * 1000 +
         (unsigned long long)(time.tv_usec) / 1000;
}
/**
 * @brief Функция проверки времени на смещение
 * @return Возвращает флаг указывающий на то, прошло ли достаточное время
 * для сдвига фигуры вниз
 *
 * Если time равно нулю, то текущее время записывается в это поле;<br>
 * Вычисляется скорость сдвига учитывая уровень;<br>
 * Проверяется, прошло ли время, равное или превышающее установленное ранее
 * время;<br> Если условия выполнены, то значение time сбрасывается в ноль.
 * Функция возвращает true, что означает, что сдвиг должен произойти.
 */
bool check_shifting(FullInfo *full_info) {
  bool flag = false;
  if (!full_info->time) {
    full_info->time = current_time();
  }
  long long unsigned int lvl_speed = 1000 / full_info->game_info.level;
  if ((current_time() - full_info->time) >= lvl_speed &&
      !full_info->game_info.pause) {
    full_info->time = 0;
    flag = true;
  }
  return flag;
}
/**
 * @brief Функция отвечающая за выделение памяти
 */
void allocate_mem(int ***matrix, int n, int m) {
  *matrix = malloc(n * sizeof(int *));
  for (int i = 0; i < n; i++) {
    (*matrix)[i] = malloc(m * sizeof(int));
    for (int j = 0; j < m; j++) {
      (*matrix)[i][j] = 0;
    }
  }
}
/**
 * @brief Функция отвечающая за очищение памяти
 */
void free_mem(int ***matrix, int n) {
  for (int i = 0; i < n; i++) {
    free((*matrix)[i]);
  }
  free(*matrix);
  *matrix = NULL;
}