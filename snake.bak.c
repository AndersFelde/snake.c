#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define yLength 20
#define xLength 30

char board[yLength][xLength];

typedef struct Coordinate {
    int x;
    int y;
} Coordinate;

typedef struct Snake {
    Coordinate head;
    Coordinate direction;
    Coordinate last;
    int length;
} Snake;

int kbhit() {
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

void createBoard() {
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
}

int snakeMove(Snake *snake) {
    snake->head.y += snake->direction.y;
    snake->head.x += snake->direction.x;

    if (board[snake->head.y][snake->head.x] == '!') {
        return 0;
    }
    return 1;
}

void updateBoard(Snake *snake) {
    clear();
    printw("%d, %d\n", snake->head.y, snake->head.x);
    printw("%d, %d\n", snake->direction.y, snake->direction.x);
    for (int y = 0; y < yLength; y++) {
        for (int x = 0; x < xLength; x++) {
            if (x == snake->head.x && y == snake->head.y) {
                printw("X");
            } else {
                printw("%c", board[y][x]);
            }
        }
        printw("\n");
    }
    usleep(1000000);
}

Snake intSnake() {
    Snake snake;

    snake.direction.x = 0;
    snake.direction.y = 1;

    snake.head.x = 2;
    snake.head.y = yLength / 2;

    snake.length = 2;

    snake.last.x = snake.head.x - 1;
    snake.last.y = snake.head.y;

    return snake;
}

int main() {
    Snake snake = intSnake();

    createBoard();

    // ncurses
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);

    while (1) {
        if (kbhit()) {
            /* Key pressed! It was: 119 W
            Key pressed! It was: 97 A
            Key pressed! It was: 115 S
            Key pressed! It was: 100 D*/
            switch (getch()) {
                // alt må være invers
                case 119:
                    if (snake.direction.y != 1) {
                        snake.direction.y = -1;
                        snake.direction.x = 0;
                    }
                    break;
                case 115:
                    if (snake.direction.y != -1) {
                        snake.direction.y = 1;
                        snake.direction.x = 0;
                    }
                    break;
                case 97:
                    if (snake.direction.x != 1) {
                        snake.direction.x = -1;
                        snake.direction.y = 0;
                    }
                    break;
                case 100:
                    if (snake.direction.x != -1) {
                        snake.direction.x = 1;
                        snake.direction.y = 0;
                    }
                    break;
                default:
                    break;
            }
        }

        refresh();

        if (!snakeMove(&snake)) {
            break;
        }
        updateBoard(&snake);
    }
}
