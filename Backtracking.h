#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include <stdio.h>

#define MAX_ROWS 100
#define MAX_COLS 100

typedef struct {
    int y, x;
} Point;

typedef struct {
    Point points[MAX_ROWS * MAX_COLS];
    int length;
} Path;

extern char maze[MAX_ROWS][MAX_COLS];
extern int visited[MAX_ROWS][MAX_COLS];
extern int rows, cols;
extern int startY, startX, endY, endX;
extern int pathCount;

void readMaze(const char *filename);
void writePath(Path *path, FILE *file);
void findPaths(int y, int x, Path *currentPath, FILE *file);
void findPeakPaths(FILE *file, int *shortestLength, char *shortestPath, int *longestLength, char *longestPath);

#endif // BACKTRACKING_H
