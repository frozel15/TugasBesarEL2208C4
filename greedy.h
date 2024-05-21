// greedy.h
#ifndef GREEDY_H
#define GREEDY_H

#define MAX 255

typedef struct {
    int x, y;
} Point_Greedy;

typedef struct {
    Point_Greedy path[MAX];
    int length;
} Path_Greedy;

void printMaze_Greedy(char maze[MAX][MAX], int row, int col);
void printPath_Greedy(Path_Greedy p);
void solveMaze(char maze[MAX][MAX], int row, int col, Path_Greedy *path, Point_Greedy start, Point_Greedy end);

#endif // GREEDY_H
