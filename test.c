#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h> /* only for sleep() */
#include <string.h>

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

int main() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    refresh();
    while (1) {
        if (kbhit()) {
            printw("Keycode: %d\n", getch());
            refresh();
        }
    }
}
