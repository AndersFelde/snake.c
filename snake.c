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
    // INFO: vurderer å ha flush input?, men vanskelig å svinke kjapt
    // flushinp();

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
        generateApple();
    }
    return 1;
}

void updateBoard(Snake *snake, WINDOW **snakeWin, WINDOW **scoreWin) {
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
    mvwprintw(*scoreWin, 0, 0, "Score: %d", score);
    wrefresh(*snakeWin);
    wrefresh(*scoreWin);

    // INFO: jo flere epler jo raskere fart
    int speedFactor = (score + 1) * 0.25;
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
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    refresh();
}

int main() {
    Snake snake = initSnake();

    initCurses();

    createBoard();
    WINDOW *borderWin = newwin(yLength, xLength, 0, 0);
    box(borderWin, 0, 0);
    wrefresh(borderWin);
    sleep(1);

    generateApple();
    // ncurses

    WINDOW *snakeWin = newwin(yLength - 2, xLength - 2, 1, 1);
    WINDOW *scoreWin = newwin(1, 20, yLength, 0);

    while (1) {
        if (kbhit()) {
            checkKey(&snake);
        }

        if (!snakeMove(&snake)) {
            break;
        }

        updateBoard(&snake, &snakeWin, &scoreWin);
        wrefresh(borderWin);
    }
    clear();
    printf("Du tapte, LOOOOL\nScore: %d", score);
}
