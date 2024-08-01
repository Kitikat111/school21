#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define WIDTH 80
#define HEIGHT 25

void inputInterface(int **map, int width, int height);
void inputFromFile(int **map);
void output(int **map, int width, int height, int cursorPosition1, int cursorPosition2);
int isEndGame(int **map, int width, int height, int ***prevMap, int *checkNext);
int startGame(int **map, int width, int height, int ***prevMap, int *checkNext, int timeout);
void gameProcess(int **map, int width, int height);
int lifeAround(int **map, int width, int height, int index1, int index2);

int main(int argc, char *argv[]) {
    int checkNext = 0;
    int timeout = 100;

    int **map = calloc(HEIGHT, sizeof(int *));
    for (int index = 0; index < HEIGHT; index++) map[index] = calloc(WIDTH, sizeof(int));

    int **prevMap = calloc(HEIGHT, sizeof(int *));
    for (int i = 0; i < HEIGHT; i++) prevMap[i] = calloc(WIDTH, sizeof(int));

    if (map != NULL && prevMap != NULL) {
        printf("input miliseconds: ");
        if (scanf("%d", &timeout) == 1) {
            if (argc > 1 && argv[1][0] == '1')
                inputInterface(map, WIDTH, HEIGHT);
            else
                inputFromFile(map);

            startGame(map, WIDTH, HEIGHT, &prevMap, &checkNext, timeout);
        } else
            printf("\033[1;31mmonly numbers\033[0m");
    } else
        printf("n/a");

    // free mem
    if (map != NULL) {
        for (int index = 0; index < HEIGHT; index++) free(map[index]);
        free(map);
    }
    if (prevMap != NULL) {
        for (int index = 0; index < HEIGHT; index++) free(prevMap[index]);
        free(prevMap);
    }
    return 0;
}

void inputInterface(int **map, int width, int height) {
    int positionHeight = height / 2, positionWidth = width / 2;
    char key = ' ';

    output(map, width, height, positionHeight, positionWidth);
    printf(
        "left: A   right: D    up: W    down: S  born/kill: L   start: X    "
        "exit: Q\n");
    do {
        key = getchar();
        if ((key == 'a' || key == 'A') && positionWidth != 0) positionWidth--;
        if ((key == 'd' || key == 'D') && positionWidth != width - 1) positionWidth++;
        if ((key == 'w' || key == 'W') && positionHeight != 0) positionHeight--;
        if ((key == 's' || key == 'S') && positionHeight != height - 1) positionHeight++;
        if (key == 'l' || key == 'L')
            map[positionHeight][positionWidth] = (map[positionHeight][positionWidth] == 0) ? 1 : 0;
        output(map, width, height, positionHeight, positionWidth);

        printf(
            "left: A   right: D    up: W    down: S  born/kill: L   start: X    "
            "exit: Q\n");
    } while (key != 'x' && key != 'X');
}

void inputFromFile(int **map) {
    int index1, index2;
    scanf("%d%d", &index1, &index2);
    while (1) {
        if (scanf("%d%d", &index1, &index2) != 2) {
            break;
        }
        map[index1][index2] = 1;
    }
}

void output(int **map, int width, int height, int cursorPosition1, int cursorPosition2) {
    system("clear");
    for (int index = 0; index < width / 2 - 3; index++) printf(" ");
    printf("Game of life\n");

    for (int index = 0; index <= width + 2; index++) printf("■");
    printf("\n");

    for (int index1 = 0; index1 < height; index1++) {
        printf("■ ");
        for (int index2 = 0; index2 < width; index2++) {
            if (index1 == cursorPosition1 && index2 == cursorPosition2) {
                if (map[index1][index2] != 0)
                    printf("\033[5;31m*\033[0m");
                else
                    printf("\033[5;31m#\033[0m");
            } else if (map[index1][index2] != 0)
                printf("\033[1;32m*\033[0m");
            else
                printf(" ");
            if (index2 == width - 1) printf("■\n");
        }
    }

    for (int index = 0; index <= width + 2; index++) printf("■");
    printf("\n");
}

int lifeAround(int **map, int width, int height, int index1, int index2) {
    int life = 0;
    int tempRow, tempCol;

    for (int row = index1 - 1; row <= index1 + 1; row++) {
        for (int col = index2 - 1; col <= index2 + 1; col++) {
            tempRow = row;
            tempCol = col;

            if (row == height || row == -1) tempRow = (row == height) ? 0 : height - 1;
            if (col == width || col == -1) tempCol = (col == width) ? 0 : width - 1;

            if (tempRow == index1 && tempCol == index2) continue;
            if (map[tempRow][tempCol] == 1) life++;
        }
    }
    return life;
}

void gameProcess(int **map, int width, int height) {
    int **nextMap = calloc(height, sizeof(int *));

    // save/revive the cell if there are 2 or 3 lives around it
    for (int index1 = 0; index1 < height; index1++) {
        nextMap[index1] = calloc(width, sizeof(int));
        for (int index2 = 0; index2 < width; index2++) {
            int life = lifeAround(map, width, height, index1, index2);

            if (map[index1][index2] == 1) {
                if (life < 2 || life > 3)
                    nextMap[index1][index2] = 0;
                else
                    nextMap[index1][index2] = 1;
            } else {
                if (life == 3)
                    nextMap[index1][index2] = 1;
                else
                    nextMap[index1][index2] = 0;
            }
        }
    }
    for (int index1 = 0; index1 < height; index1++)
        for (int index2 = 0; index2 < width; index2++) map[index1][index2] = nextMap[index1][index2];

    for (int index1 = 0; index1 < height; index1++) free(nextMap[index1]);
    free(nextMap);
}

// start the game
int startGame(int **map, int width, int height, int ***prevMap, int *checkNext, int timeout) {
    while (!isEndGame(map, width, height, prevMap, checkNext)) {
        gameProcess(map, width, height);
        output(map, width, height, -1, -1);
        usleep(timeout * 1000);
    }
    printf("Игра завершена\n");
    return 0;
}

int isEndGame(int **map, int width, int height, int ***prevMap, int *checkNext) {
    // count of living cells
    int aliveCells = 0;
    for (int index1 = 0; index1 < height; index1++)
        for (int index2 = 0; index2 < width; index2++)
            if (map[index1][index2] == 1) aliveCells++;

    if (aliveCells == 0) return 1;

    if (*checkNext == 1) {
        int sameState = 1;
        for (int index1 = 0; index1 < height; index1++) {
            for (int index2 = 0; index2 < width; index2++)
                if (map[index1][index2] != prevMap[0][index1][index2]) {
                    sameState = 0;
                    break;
                }
            if (!sameState) break;
        }
        if (sameState) return 1;
        *checkNext = 0;
    } else
        *checkNext = 1;

    if (*checkNext == 0)
        for (int index1 = 0; index1 < height; index1++)
            for (int index2 = 0; index2 < width; index2++) prevMap[0][index1][index2] = map[index1][index2];

    return 0;
}
