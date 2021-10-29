#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED
#include "board.h"
#include "tools.h"

typedef struct Last {
    Coordinate cord;
    struct Last *next;
} Last;

typedef struct Snake {
    Coordinate head;
    Coordinate direction;
    Last *lastLast;
    Last *firstLast;
    Coordinate nextLast;
    int length;
} Snake;

int snakeMove(Snake *snake, int *score, char board[yLength][xLength],
              Coordinate *apple);

Snake initSnake(char board[yLength][xLength]);
#endif
