#include <ncurses.h>

#include "snake.h"
#include "tools.h"

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

void checkKey(Snake *snake) {
    /* Key pressed! It was: 119 W
    Key pressed! It was: 97 A
    Key pressed! It was: 115 S
    Key pressed! It was: 100 D*/
    /* Opp:
    Keycode: 27
    Keycode: 91
    Keycode: 65
    Høyre:
    Keycode: 27
    Keycode: 91
    Keycode: 67
    Ned:
    Keycode: 27
    Keycode: 91
    Keycode: 66
    Venstre:
    Keycode: 27
    Keycode: 91
    Keycode: 68 */
    int key = getch();
    if (key == 27) {  // fordi piltaster er rart
        getch();
        key = getch();
    }

    switch (key) {
        // alt må være invers
        case 119:
        case 65:
            if (snake->direction.y != 1) {
                snake->direction.y = -1;
                snake->direction.x = 0;
            }
            break;
        case 115:
        case 66:
            if (snake->direction.y != -1) {
                snake->direction.y = 1;
                snake->direction.x = 0;
            }
            break;
        case 97:
        case 68:
            if (snake->direction.x != 1) {
                snake->direction.x = -1;
                snake->direction.y = 0;
            }
            break;
        case 100:
        case 67:
            if (snake->direction.x != -1) {
                snake->direction.x = 1;
                snake->direction.y = 0;
            }
            break;
        default:
            break;
    }
}
