// dijkstra.h
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <limits.h>
#include <time.h>

#define MAX_ROW 300
#define MAX_COL 300
#define INF INT_MAX

typedef struct {
    int x;
    int y;
} Graph;

void SetInf(int element[MAX_ROW][MAX_COL], int row, int col);
int valid(int x, int y, int row, int col);
void printPath_Dijkstra(Graph path[MAX_ROW][MAX_COL], Graph start, Graph end, char maze[MAX_ROW][MAX_COL], int row, int col);
void Dijkstra(char maze[MAX_ROW][MAX_COL], int row, int col, Graph start, Graph end);
double CalcTime(clock_t start, clock_t finish);
void readMazeFile(char *filename, char maze[MAX_ROW][MAX_COL], int *rows, int *cols);

#endif // DIJKSTRA_H