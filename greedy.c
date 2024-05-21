// greedy.c
#include <stdio.h>
#include <stdbool.h>
#include "greedy.h"

void printMaze_Greedy(char maze[MAX][MAX], int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }
}

void printPath_Greedy(Path_Greedy p) {
    for (int i = 0; i < p.length; i++) {
        printf("(%d,%d)", p.path[i].x, p.path[i].y);
        if (i < p.length - 1) printf("->");
    }
    printf("\n");
}

void solveMaze(char maze[MAX][MAX], int row, int col, Path_Greedy *path, Point_Greedy start, Point_Greedy end) {
    int solution[MAX][MAX];
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            solution[i][j] = 0;
        }
    }

    int x = start.x, y = start.y;
    solution[x][y] = 1;
    path->path[0] = (Point_Greedy){x, y};
    path->length = 1;

    while (maze[x][y] != 'E') {
        bool moved = false;

        if (end.x > start.x && end.y > start.y) {
            if (maze[x + 1][y] != '#' && x < row - 1) {
                x++;
                moved = true;
            } 
            else if (maze[x][y + 1] != '#' && y < col - 1) {
                y++;
                moved = true;
            }
        } else if (end.x > start.x && end.y < start.y) {
            if (maze[x + 1][y] != '#' && x < row - 1) {
                x++;
                moved = true;
            } 
            else if (maze[x][y - 1] != '#' && y > 0) {
                y--;
                moved = true;
            }
        } else if (end.x < start.x && end.y > start.y) {
            if (maze[x - 1][y] != '#' && x > 0) {
                x--;
                moved = true;
            } 
            else if (maze[x][y + 1] != '#' && y < col - 1) {
                y++;
                moved = true;
            }
        } else if (end.x < start.x && end.y < start.y) {
            if (maze[x - 1][y] != '#' && x > 0) {
                x--;
                moved = true;
            } 
            else if (maze[x][y - 1] != '#' && y > 0) {
                y--;
                moved = true;
            }
        }

        if (!moved) {
            break;
        }

        solution[x][y] = 1;
        path->path[path->length] = (Point_Greedy){x, y};
        path->length++;
    }

    if (maze[x][y] == 'E') {
        path->path[path->length] = (Point_Greedy){x, y};
        path->length++;
    }

    printf("Solusi Maze:\n");
    printMaze_Greedy(maze, row, col);
}
