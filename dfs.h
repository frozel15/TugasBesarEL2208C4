#ifndef DFS_H
#define DFS_H

#include <stdbool.h>

#define MAX 100

extern int found;
extern int pathquantity;
extern int longestPathLength;
extern int longestPath[MAX][MAX];

bool bacaLabirin(char *namaFile, char labirin[MAX][MAX], int *baris, int *kolom);
void resetMap(char labirin[MAX][MAX]);
void printMap(char labirin[MAX][MAX], int kolom, int baris);
void findRoute(char labirin[MAX][MAX], int x, int y, int counter, int baris, int kolom);
void tracing(char labirin[MAX][MAX], int x, int y, int baris, int kolom);
void tracingAllPaths(char labirin[MAX][MAX], int x, int y, int baris, int kolom, int path[MAX][MAX], int idx);
void findLongestPath(char labirin[MAX][MAX], int x, int y, int baris, int kolom, int path[MAX][MAX], int idx);

#endif
