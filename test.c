#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h> /* only for sleep() */
#include <unistd.h> /* only for sleep() */

int kbhit(void) {
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

int main(void) {
    initscr();

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    scrollok(stdscr, TRUE);
    int i = 0;
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
    }
}
