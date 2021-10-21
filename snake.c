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
    printf("Tried to move");
    snake->lastLast = snake->lastLast->next;

    Last firstLast;
    firstLast.cord.x = snake->head.x;
    firstLast.cord.y = snake->head.y;

    snake->firstLast->next = &firstLast;
    snake->firstLast = &firstLast;

    snake->head.y += snake->direction.y;
    snake->head.x += snake->direction.x;

    board[snake->head.y][snake->head.x] = 'X';  // flytter hodet
    board[snake->lastLast->cord.y][snake->lastLast->cord.x] =
        ' ';  // fjerner siste enden

    if (board[snake->head.y][snake->head.x] == '!') {
        return 0;
    }
    return 1;
}

void updateBoard(Snake *snake) {
    // clear();
    printw("%d, %d\n", snake->head.y, snake->head.x);
    printw("%d, %d\n", snake->lastLast->cord.y, snake->lastLast->cord.x);
    printw("%d, %d\n", snake->firstLast->cord.y, snake->firstLast->cord.x);
    printw("%d, %d\n", snake->direction.y, snake->direction.x);
    for (int y = 0; y < yLength; y++) {
        for (int x = 0; x < xLength; x++) {
            printw("%c", board[y][x]);
        }
        printw("\n");
    }
    usleep(1000000);
}

Snake initSnake() {
    Snake snake;

    snake.direction.x = 1;
    snake.direction.y = 0;

    snake.head.x = 3;
    snake.head.y = yLength / 2;

    snake.length = 3;

    Last lastLast;
    Last firstLast;

    lastLast.cord.x = snake.head.x - 2;
    lastLast.cord.y = snake.head.y;
    lastLast.next = &firstLast;

    firstLast.cord.x = snake.head.x - 1;
    firstLast.cord.y = snake.head.y;

    snake.lastLast = &lastLast;
    snake.firstLast = &firstLast;

    board[snake.head.y][snake.head.x] = 'X';  // flytter hodet
    board[snake.firstLast->cord.y][snake.firstLast->cord.x] = 'X';
    board[snake.lastLast->cord.y][snake.lastLast->cord.y] = 'X';

    return snake;
}

void checkKey(Snake *snake) {
    switch (getch()) {
        // alt må være invers
        case 119:
            if (snake->direction.y != 1) {
                snake->direction.y = -1;
                snake->direction.x = 0;
            }
            break;
        case 115:
            if (snake->direction.y != -1) {
                snake->direction.y = 1;
                snake->direction.x = 0;
            }
            break;
        case 97:
            if (snake->direction.x != 1) {
                snake->direction.x = -1;
                snake->direction.y = 0;
            }
            break;
        case 100:
            if (snake->direction.x != -1) {
                snake->direction.x = 1;
                snake->direction.y = 0;
            }
            break;
        default:
            break;
    }
}

void initCurses() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);
}

int main() {
    printf("started");
    // Snake snake = initSnake();
    Snake snake;

    createBoard();
    // initCurses();

    // ncurses

    while (1) {
        if (kbhit()) {
            /* Key pressed! It was: 119 W
            Key pressed! It was: 97 A
            Key pressed! It was: 115 S
            Key pressed! It was: 100 D*/
            checkKey(&snake);
        }

        refresh();

        if (!snakeMove(&snake)) {
            break;
        }
        updateBoard(&snake);
    }
}
