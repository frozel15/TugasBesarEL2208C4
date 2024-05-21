#include "dfs.h"
#include <stdio.h>
#include <stdbool.h>

#define MAX 255

int found = 0;
int pathquantity = 0;
int longestPathLength = 0;
int longestPath[MAX][MAX] = {0};

bool bacaLabirin(char *namaFile, char labirin[MAX][MAX], int *baris, int *kolom) {
    FILE *file = fopen(namaFile, "r");
    if (file == NULL) {
        printf("File tidak dapat dibuka.\n");
        return false;
    }

    char ch;
    *baris = 0;
    *kolom = 0;
    int kolomSementara = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') { 
            if (*baris == 0) {
                *kolom = kolomSementara;
            }
            kolomSementara = 0;
            (*baris)++;
        } else {
            if (ch == '#') {
                labirin[*baris][kolomSementara] = -3;
            } else if (ch == 'S') {
                labirin[*baris][kolomSementara] = -1;
            } else if (ch == 'E') {
                labirin[*baris][kolomSementara] = -4;
            }
            kolomSementara++;
        }
    }
    if (kolomSementara != 0) {
        (*baris)++;
    }

    fclose(file);
    return true;
}

void resetMap(char labirin[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            labirin[i][j] = 0;
        }
    }
}

void printMap(char labirin[MAX][MAX], int kolom, int baris) {
    for (int i = 0; i < baris; i++) {
        for (int j = 0; j < kolom; j++) {
            if (labirin[i][j] == -3) {
                printf("%s", "()");
            } else if (labirin[i][j] == -4) {
                printf("%s", "EE");
            } else {
                printf("%02d", labirin[i][j]);
            }
        }
        printf("\n");
    }
}

void findRoute(char labirin [MAX] [MAX], int x, int y, int counter, int baris, int kolom) {
    if (labirin[y][x] == -3 || labirin[y][x] ==  -1 || x < 0 || x >= kolom || y < 0 || y >= baris) {
        return;
    }

    if (labirin[y][x] <= counter && labirin[y][x] > 0) {
        return;
    }

    labirin[y][x] = counter;

    findRoute(labirin , x+1, y, counter+1, baris, kolom);
    findRoute(labirin , x, y+1, counter+1, baris, kolom);
    findRoute(labirin, x-1, y, counter+1, baris, kolom);
    findRoute(labirin , x, y-1, counter+1, baris, kolom);
}

void tracing(char labirin[MAX][MAX], int x, int y, int baris, int kolom, int length) {
    int min = 99999;

    if (labirin[y][x] == -3) {
        return;
    }

    if (labirin[y][x] == 1) {
        printf("(%d, %d)\nLength = %d\n", y, x, length);
        found = 1;
        return;
    }

    printf("(%d, %d) -> ", y, x);

    if (found == 1) {
        return;
    }

    if (x+1 < kolom && labirin[y][x+1] < min && labirin[y][x+1] > 0) {
        min = labirin[y][x+1];
    }

    if (x > 0 && labirin[y][x-1] < min && labirin[y][x-1] > 0) {
        min = labirin[y][x-1];
    }

    if (y+1 < baris && labirin[y+1][x] < min && labirin[y+1][x] > 0) {
        min = labirin[y+1][x];
    }

    if (y > 0 && labirin[y-1][x] < min && labirin[y-1][x] > 0) {
        min = labirin[y-1][x];
    }

    if (min == labirin[y][x+1] && found == 0) {
        tracing(labirin, x+1, y, baris, kolom, length+1);
    } else if (min == labirin[y][x-1] && found == 0) {
        tracing(labirin, x-1, y, baris, kolom, length+1);
    } else if (min == labirin[y+1][x] && found == 0) {
        tracing(labirin, x, y+1, baris, kolom, length+1);
    } else if (min == labirin[y-1][x] && found == 0) {
        tracing(labirin, x, y-1, baris, kolom, length+1);
    }
}

void tracingAllPaths(char labirin[MAX][MAX], int x, int y, int baris, int kolom, int path[MAX][MAX], int idx) {
    path[idx][0] = y;
    path[idx][1] = x;
    idx++;

    if (labirin[y][x] == 1) {
        pathquantity+=1;
        for (int i = 0; i < idx; i++) {
            if (i == idx - 1) {
                printf("(%d, %d)", path[i][0], path[i][1]);
            } else {
                printf("(%d, %d) -> ", path[i][0], path[i][1]);
            }
        }
        printf("\n");
        return;
    }

    int temp = labirin[y][x];
    labirin[y][x] = -2;

    if (temp == -1) {
        if (x+1 < kolom && labirin[y][x+1] != -2) {
            tracingAllPaths(labirin, x+1, y, baris, kolom, path, idx);
        }
        if (x > 0 && labirin[y][x-1] != -2) {
            tracingAllPaths(labirin, x-1, y, baris, kolom, path, idx);
        }
        if (y+1 < baris && labirin[y+1][x] != -2) {
            tracingAllPaths(labirin, x, y+1, baris, kolom, path, idx);
        }
        if (y > 0 && labirin[y-1][x] != -2) {
            tracingAllPaths(labirin, x, y-1, baris, kolom, path, idx);
        }
    } else {
        if (x+1 < kolom && labirin[y][x+1] != -2 && labirin[y][x+1] == temp - 1) {
            tracingAllPaths(labirin, x+1, y, baris, kolom, path, idx);
        }
        if (x > 0 && labirin[y][x-1] != -2 && labirin[y][x-1] == temp - 1) {
            tracingAllPaths(labirin, x-1, y, baris, kolom, path, idx);
        }
        if (y+1 < baris && labirin[y+1][x] != -2 && labirin[y+1][x] == temp - 1) {
            tracingAllPaths(labirin, x, y+1, baris, kolom, path, idx);
        }
        if (y > 0 && labirin[y-1][x] != -2 && labirin[y-1][x] == temp - 1) {
            tracingAllPaths(labirin, x, y-1, baris, kolom, path, idx);
        }
    }

    labirin[y][x] = temp;
}

void findLongestPath(char labirin[MAX][MAX], int x, int y, int baris, int kolom, int path[MAX][MAX], int idx) {
    path[idx][0] = y;
    path[idx][1] = x;
    idx++;
    FILE* allPath = fopen("allPath.txt", "a");
    if (labirin[y][x] == -4) {
        pathquantity+=1;
        for (int i = 0; i < idx; i++) {
            if (i == idx - 1) {
                fprintf(allPath, "(%d, %d)", path[i][0], path[i][1]);
            } else {
                fprintf(allPath, "(%d, %d) -> ", path[i][0], path[i][1]);
            }
        }
        fprintf(allPath, "\n");
        fclose(allPath);

        if (idx > longestPathLength) {
            longestPathLength = idx;
            for (int i = 0; i < idx; i++) {
                longestPath[i][0] = path[i][0];
                longestPath[i][1] = path[i][1];
            }
        }
        return;
    }
    fclose(allPath);

    int temp = labirin[y][x];
    labirin[y][x] = -2;

    if (x+1 < kolom && (labirin[y][x+1] == 0 || labirin[y][x+1] == -4)) {
        findLongestPath(labirin, x+1, y, baris, kolom, path, idx);
    }
    if (x > 0 && (labirin[y][x-1] == 0 || labirin[y][x-1] == -4)) {
        findLongestPath(labirin, x-1, y, baris, kolom, path, idx);
    }
    if (y+1 < baris && (labirin[y+1][x] == 0 || labirin[y+1][x] == -4)) {
        findLongestPath(labirin, x, y+1, baris, kolom, path, idx);
    }
    if (y > 0 && (labirin[y-1][x] == 0 || labirin[y-1][x] == -4)) {
        findLongestPath(labirin, x, y-1, baris, kolom, path, idx);
    }

    labirin[y][x] = temp;
}
