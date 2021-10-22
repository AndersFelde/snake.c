#include <stdio.h>

int main() {
    int input;
    scanf("%d", &input);

    int output[input][input];

    int counter = 1;
    for (int y = 0; y < input; y++) {
        for (int x = 0; x < input; x++) {
            output[y][x] = counter;
            counter++;
        }
    }

    for (int y = 0; y < input; y++) {
        for (int x = 0; x < input; x++) {
            printf("%02d ", output[y][x]);
        }
        printf("\n");
    }

    return 0;
}
