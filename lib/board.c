#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "board.h"
#include "snake.h"

void createBoard(char board[yLength][xLength], Coordinate *apple) {
    for (int y = 0; y < yLength; y++) {
        for (int x = 0; x < xLength; x++) {
            if (y == 0 || y == yLength - 1) {
                board[y][x] = '!';
            } else if (x == 0 || x == xLength - 1) {
                board[y][x] = '!';
            } else {
                board[y][x] = ' ';
            }
        }
    }
    generateApple(apple, board);
}

void generateApple(Coordinate *apple, char board[yLength][xLength]) {
    // rand() % (max_number + 1 - minimum_number) + minimum_number
    // BUG: kan spawne inne i slangen!!
    do {
        srand(time(NULL));
        apple->x = rand() % (xLength - 1 - 1) + 1;
        apple->y = rand() % (yLength - 1 - 1) + 1;
    } while (board[apple->y][apple->x] == 'X');

    board[apple->y][apple->x] = '*';
}

void updateBoard(Snake *snake, WINDOW **snakeWin, WINDOW **scoreWin,
                 char board[yLength][xLength], int *score) {
    // wclear(*snakeWin);
    for (int y = 0; y < yLength - 2; y++) {
        for (int x = 0; x < xLength - 2; x++) {
            char tile = board[y + 1][x + 1];
            // BUG: farger funker ikke
            if (snake->head.y - 1 == y && snake->head.x - 1 == x) {
                mvwaddch(*snakeWin, y, x, '+' | COLOR_PAIR(1));
            } else {
                int color;
                if (tile == '*') {
                    color = COLOR_PAIR(2);
                } else {
                    color = COLOR_PAIR(1);
                }
                mvwaddch(*snakeWin, y, x, board[y + 1][x + 1] | color);
            }
        }
    }
    mvwprintw(*scoreWin, 0, 0, "Score: %d", *score);
    wrefresh(*snakeWin);
    wrefresh(*scoreWin);

    // INFO: jo flere epler jo raskere fart
    int speedFactor = (*score + 1) * 0.25;
    if (snake->direction.y == 0) {
        usleep(100000);
    } else {
        usleep(150000);
    }
}
