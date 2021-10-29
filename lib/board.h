#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED
#define xLength 30
#define yLength 20

#include <ncurses.h>

typedef struct Snake Snake;
typedef struct Coordinate Coordinate;

void createBoard(char board[yLength][xLength], Coordinate *apple);
void generateApple(Coordinate *apple, char board[yLength][xLength]);
void updateBoard(Snake *snake, WINDOW **snakeWin, WINDOW **scoreWin,
                 char board[yLength][xLength], int *score);
#endif
