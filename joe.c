#include <stdio.h>

void readFile() {
    FILE *f;
    int bufferSize = 100;
    char buffer[bufferSize];
    f = fopen("HEI.txt", "r");

    while (fgets(buffer, bufferSize, f)) {
        printf("%s", buffer);
    }
    fclose(f);
    return;
}

void lesFil() {
    FILE *fp;
    char string[60];
    fp = fopen("HEI.txt", "r");
    fgets(string, 60, fp);
    printf("%s", string);
    fclose(fp);
    return;
}

int main() {
    FILE *f;
    f = fopen("HEI.txt", "w+");

    fprintf(f, "Hei Verden 2020.\nVelkommen til min kode.");
    fclose(f);
    // readFile();
    lesFil();
    return 0;
}
