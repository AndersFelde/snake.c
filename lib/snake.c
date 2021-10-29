#include <stdlib.h>

#include "board.h"
#include "snake.h"

int snakeMove(Snake *snake, int *score, char board[yLength][xLength],
              Coordinate *apple) {
    int eatApple = 0;
    char nextTile = board[snake->head.y + snake->direction.y]
                         [snake->head.x + snake->direction.x];
    if (nextTile == '!' || nextTile == 'X') {
        return 0;
    } else if (nextTile == '*') {
        eatApple = 1;
        *score = *score + 1;
    }
    // BUG: WHAT?????? printf("moving\n");

    Last *firstLast = (Last *)calloc(1, sizeof(Last));
    firstLast->cord.x = snake->head.x;
    firstLast->cord.y = snake->head.y;

    snake->firstLast->next = firstLast;
    snake->firstLast = firstLast;

    snake->head.y += snake->direction.y;
    snake->head.x += snake->direction.x;

    board[snake->head.y][snake->head.x] = 'X';

    if (!eatApple) {
        board[snake->lastLast->cord.y][snake->lastLast->cord.x] =
            ' ';  // fjerner siste enden
        Last *nextLastLast = snake->lastLast->next;
        free(snake->lastLast);
        snake->lastLast = nextLastLast;
    } else {
        generateApple(apple, board);
    }
    return 1;
}

Snake initSnake(char board[yLength][xLength]) {
    Snake snake;

    snake.direction.x = 1;
    snake.direction.y = 0;

    snake.head.x = 4;
    snake.head.y = yLength / 2;

    snake.length = 3;

    Last *lastLast = (Last *)calloc(1, sizeof(Last));
    Last *firstLast = (Last *)calloc(1, sizeof(Last));

    lastLast->cord.x = snake.head.x - 2;
    lastLast->cord.y = snake.head.y;
    lastLast->next = firstLast;

    firstLast->cord.x = snake.head.x - 1;
    firstLast->cord.y = snake.head.y;

    snake.lastLast = lastLast;
    snake.firstLast = firstLast;

    board[snake.head.y][snake.head.x] = 'X';  // flytter hodet
    board[snake.firstLast->cord.y][snake.firstLast->cord.x] = 'X';
    board[snake.lastLast->cord.y][snake.lastLast->cord.x] = 'X';

    return snake;
}
