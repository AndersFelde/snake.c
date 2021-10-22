#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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

#define yLength 20
#define xLength 30

char board[yLength][xLength];
Coordinate apple;
int score;

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
            printw("%c", board[y][x]);
        }
        printw("\n");
    }
}

void generateApple() {
    // rand() % (max_number + 1 - minimum_number) + minimum_number
    // BUG: kan spawne inne i slangen!!
    do {
        srand(time(NULL));
        apple.x = rand() % (xLength - 1 - 1) + 1;
        apple.y = rand() % (yLength - 1 - 1) + 1;
    } while (board[apple.y][apple.x] == 'X');

    board[apple.y][apple.x] = '*';
}

int snakeMove(Snake *snake) {
    int eatApple = 0;
    char nextTile = board[snake->head.y + snake->direction.y]
                         [snake->head.x + snake->direction.x];
    if (nextTile == '!' || nextTile == 'X') {
        return 0;
    } else if (nextTile == '*') {
        eatApple = 1;
        score++;
    }
    // BUG: WHAT?????? printf("moving\n");

    /* printf("%p\n", snake->lastLast->next);
    printf("%p\n", snake->firstLast); */
    // printf("FirstLast: %p\n", snake->firstLast);
    // printf("FirstLast:%d,%d\n", snake->firstLast->cord.x,
    //        snake->firstLast->cord.y);
    // printf("lastLast:%d,%d\n", snake->lastLast->cord.x,
    //        snake->lastLast->cord.y);
    // printf("lastLast: %p\n", snake->lastLast);

    // BUG: Blir ikke til ny variabel andre gangen
    Last *firstLast = (Last *)calloc(1, sizeof(Last));
    firstLast->cord.x = snake->head.x;
    firstLast->cord.y = snake->head.y;

    snake->firstLast->next = firstLast;
    // printf("1:%d,%d\n", snake->firstLast->cord.x, snake->firstLast->cord.y);
    snake->firstLast = firstLast;
    // printf("1:%d,%d\n", snake->firstLast->cord.x, snake->firstLast->cord.y);

    // snake->lastLast = snake->lastLast->next;
    // printf("2:%d,%d\n", snake->lastLast->cord.x, snake->lastLast->cord.y);

    snake->head.y += snake->direction.y;
    snake->head.x += snake->direction.x;

    board[snake->head.y][snake->head.x] = 'X';

    if (!eatApple) {
        board[snake->lastLast->cord.y][snake->lastLast->cord.x] =
            ' ';  // fjerner siste enden
        // printf("1:%d,%d\n", snake->lastLast->cord.x,
        // snake->lastLast->cord.y);
        Last *nextLastLast = snake->lastLast->next;
        free(snake->lastLast);
        snake->lastLast = nextLastLast;
    } else {
        generateApple();
    }
    // flytter hodet

    /* printf("%p\n", snake->lastLast->next);
    printf("%p\n", snake->firstLast); */
    // printf("%d,%d\n", snake->lastLast->cord.x, snake->lastLast->cord.y);
    // printf("%d,%d\n", snake->firstLast->cord.x, snake->firstLast->cord.y);
    // printf("%d,%d\n", snake->head.x, snake->head.y);
    return 1;
}

void updateBoard(Snake *snake) {
    clear();
    printw("%d, %d\n", apple.y, apple.x);
    printw("Score: %d\n", score);

    // printw("%d, %d\n", snake->head.y, snake->head.x);
    // printw("%d, %d\n", snake->firstLast->cord.y, snake->firstLast->cord.x);
    // printw("%d, %d\n", snake->lastLast->cord.y, snake->lastLast->cord.x);
    // printw("%d, %d\n", snake->direction.y, snake->direction.x);
    // printw("%c\n", board[snake->head.y][snake->head.x]);
    for (int y = 0; y < yLength; y++) {
        for (int x = 0; x < xLength; x++) {
            if (snake->head.y == y && snake->head.x == x) {
                printw("+");
            } else {
                printw("%c", board[y][x]);
            }
        }
        printw("\n");
    }
    if (snake->direction.y == 0) {
        usleep(100000);
    } else {
        usleep(150000);
    }
}

Snake initSnake() {
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

void checkKey(Snake *snake) {
    /* Key pressed! It was: 119 W
    Key pressed! It was: 97 A
    Key pressed! It was: 115 S
    Key pressed! It was: 100 D*/
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
    Snake snake = initSnake();

    // ncurses
    initCurses();

    createBoard();
    generateApple();
    while (1) {
        if (kbhit()) {
            checkKey(&snake);
        }

        refresh();

        // printf("%p\n", snake.lastLast->next);
        // printf("%p\n", snake.firstLast);
        if (!snakeMove(&snake)) {
            break;
        }

        updateBoard(&snake);
    }
}
