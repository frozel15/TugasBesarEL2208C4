#include "Backtracking.h"
#include <stdlib.h>
#include <string.h>

char maze[MAX_ROWS][MAX_COLS];
int visited[MAX_ROWS][MAX_COLS];
int rows, cols;
int startY, startX, endY, endX;
int pathCount = 0; // Counter for the number of paths

void readMaze(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("\nFile doesn't exist.\n");
        exit(1);
    }

    rows = 0;
    int sCount = 0, eCount = 0;
    while (fgets(maze[rows], MAX_COLS, file)) {
        cols = strlen(maze[rows]);
        if (maze[rows][cols - 1] == '\n') maze[rows][cols - 1] = '\0';
        cols = strlen(maze[rows]);
        for (int x = 0; x < cols; x++) {
            if (maze[rows][x] == 'S') {
                startY = rows;
                startX = x;
                sCount++;
            } else if (maze[rows][x] == 'E') {
                endY = rows;
                endX = x;
                eCount++;
            }
        }
        rows++;
    }
    fclose(file);

    if (sCount != 1 || eCount != 1) {
        printf("\nMaze must have exactly one start point (S) and one end point (E).\n");
        exit(1);
    }
}

void writePath(Path *path, FILE *file) {
    fprintf(file, "Path %d: ", pathCount + 1);
    for (int i = 0; i < path->length; i++) {
        fprintf(file, "(%d, %d)", path->points[i].y, path->points[i].x);
        if (i < path->length - 1) {
            fprintf(file, " -> ");
        }
    }
    fprintf(file, "\n");
}

void findPaths(int y, int x, Path *currentPath, FILE *file) {
    if (y < 0 || y >= rows || x < 0 || x >= cols || maze[y][x] == '#' || visited[y][x]) {
        return;
    }

    currentPath->points[currentPath->length].y = y;
    currentPath->points[currentPath->length].x = x;
    currentPath->length++;

    if (y == endY && x == endX) {
        writePath(currentPath, file);
        pathCount++;
    } else {
        visited[y][x] = 1;

        findPaths(y + 1, x, currentPath, file); // Move down
        findPaths(y - 1, x, currentPath, file); // Move up
        findPaths(y, x + 1, currentPath, file); // Move right
        findPaths(y, x - 1, currentPath, file); // Move left

        visited[y][x] = 0;
    }

    currentPath->length--;
}

void findPeakPaths(FILE *file, int *shortestLength, char *shortestPath, int *longestLength, char *longestPath) {
    char line[1024];
    *shortestLength = -1;
    *longestLength = -1;

    rewind(file);
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Path", 4) == 0) {
            int length = 1; // At least one point
            for (char *p = line; *p; p++) {
                if (*p == '>') length++;
            }

            if (*shortestLength == -1 || length < *shortestLength) {
                *shortestLength = length;
                strcpy(shortestPath, line);
            }

            if (*longestLength == -1 || length > *longestLength) {
                *longestLength = length;
                strcpy(longestPath, line);
            }
        }
    }
}
