#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h> /* only for sleep() */
#include <unistd.h> /* only for sleep() */

#define yLength 20
#define xLength 30

char board[yLength][xLength];
int kbhit(void) {
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

void drawBoard() {
    for (int y = 0; y < yLength; y++) {
        for (int x = 0; x < xLength; x++) {
            printw("%c", board[y][x]);
        }
        printw("\n");
    }
}

int main(void) {
    initscr();

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    scrollok(stdscr, TRUE);

    createBoard();
    drawBoard();
    refresh();
    sleep(2);

    WINDOW *win_self = newwin(yLength - 2, xLength - 2, 1, 1);
    // box(win_self, 0, 0);

    while (1) {
        for (int y = 0; y < yLength - 2; y++) {
            for (int x = 0; x < xLength - 2; x++) {
                mvwaddch(win_self, y, x, 'A');
            }
        }
        wrefresh(win_self);
        usleep(100000);
        wclear(win_self);
        wrefresh(win_self);
        usleep(100000);
    }
    sleep(3);
    wclear(win_self);
    wrefresh(win_self);
    sleep(3);
    /* int i = 0;
    while (1) {
        if (kbhit()) {
            // clear();
            printw("Key pressed! It was: %d\n", getch());
            i++;
            refresh();
        } else {
            // clear();
            printw("No key pressed yet %d\n", i);
            refresh();
            sleep(1);
        }
    } */
}
