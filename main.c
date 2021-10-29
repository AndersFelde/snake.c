#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "lib/board.h"
#include "lib/snake.h"

#define usernameLength 20
#define highScoreLength 10

typedef struct Highscore {
    char username[usernameLength];
    char highScore[highScoreLength];
} Highscore;

int score = 0;

void initCurses() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    flushinp();
    refresh();
}

void writeHighScore(Highscore *user) {
    char lengthString[highScoreLength];
    int length;
    int i = 0;
    FILE *file = fopen("highscore.txt", "r");
    char *tempUsername;
    char *tempScore;
    char temp[usernameLength + highScoreLength + 1];
    int isHigher = 0;
    sprintf(user->highScore, "%d", score);

    printf("--------HIGHSCORE--------\n");
    if (!file) {  // sjekker om filen finnes
        file = fopen("highscore.txt", "w+");
        fprintf(file, "1\n");
        fprintf(file, "%s:%s\n", user->username, user->highScore);
        printf("%s:%s <--\n", user->username, user->highScore);
        fclose(file);
        return;
    }

    fgets(lengthString, highScoreLength, file);
    length = atoi(lengthString) + 1;  // fordi skal ha en ny score også

    char highScores[length][highScoreLength + usernameLength +
                            1];  // maks lengde på highscore, 10 tall

    while (fgets(highScores[i], highScoreLength + usernameLength + 1,
                 file) &&
           i < length) {  // leser input og putter det i highScores array, den
                          // er ferdig sortert
        strcpy(temp, highScores[i]);
        tempUsername = strtok(temp, ":");
        tempScore = strtok(NULL, ":");
        tempScore[strcspn(tempScore, "\n")] = 0;  // for å fjerne newline

        if (score >= atoi(tempScore) && !isHigher) {
            isHigher = 1;
            sprintf(highScores[i], "%s:%s\n", user->username, user->highScore);
            printf("%s:%s <--\n", user->username, user->highScore);

            i++;
            sprintf(highScores[i], "%s:%s\n", tempUsername, tempScore);
        }
        printf("%s", highScores[i]);
        i++;
    }

    if (!isHigher) {
        sprintf(highScores[i], "%s:%s\n", user->username, user->highScore);
        printf("%s:%s <--\n", user->username, user->highScore);
    }

    fclose(file);
    file = fopen("highscore.txt", "w+");

    fprintf(file, "%d\n", length);

    for (int x = 0; x < length; x++) {
        fprintf(file, "%s", highScores[x]);
    }

    fclose(file);
    return;
}

int main() {
    char board[yLength][xLength];
    Coordinate apple;

    Snake snake = initSnake(board);
    Highscore *user = calloc(1, sizeof(Highscore));

    printf("Ditt navn: ");
    fgets(user->username, usernameLength, stdin);
    user->username[strcspn(user->username, "\n")] = 0;  // for å fjerne newline

    initCurses();

    createBoard(board, &apple);
    WINDOW *borderWin = newwin(yLength, xLength, 0, 0);
    box(borderWin, 0, 0);
    wrefresh(borderWin);
    sleep(1);

    // ncurses

    WINDOW *snakeWin = newwin(yLength - 2, xLength - 2, 1, 1);
    WINDOW *scoreWin = newwin(1, 20, yLength, 0);

    while (1) {
        if (kbhit()) {
            checkKey(&snake);
        }

        if (!snakeMove(&snake, &score, board, &apple)) {
            break;
        }

        updateBoard(&snake, &snakeWin, &scoreWin, board, &score);
        wrefresh(borderWin);
    }
    endwin();
    clear();
    writeHighScore(user);
    // system("stty sane");
    // printf("Du tapte, LOOOOL\nScore: %d", score);
}
