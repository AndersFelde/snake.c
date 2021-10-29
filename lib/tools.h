#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

typedef struct Snake Snake;

typedef struct Coordinate {
    int x;
    int y;
} Coordinate;

int kbhit();
void checkKey(Snake *snake);
void initCurses();
// void writeHighScore(Highscore *user);

#endif
