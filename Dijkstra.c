// dijkstra.c
#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void SetInf(int element[MAX_ROW][MAX_COL], int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            element[i][j] = INF;
        }
    }
}

int valid(int x, int y, int row, int col) {
    return (x >= 0 && x < row && y >= 0 && y < col);
}

void printPath_Dijkstra(Graph path[MAX_ROW][MAX_COL], Graph start, Graph end, char maze[MAX_ROW][MAX_COL], int row, int col) {
    if (start.x == end.x && start.y == end.y) {
        maze[end.x][end.y] = 'S';
        printf("(%d, %d)", start.x, start.y);
        return;
    }
    printPath_Dijkstra(path, start, path[end.x][end.y], maze, row, col);
    printf(" -> (%d, %d)", end.x, end.y);
    maze[end.x][end.y] = 'x';
}

void Dijkstra(char maze[MAX_ROW][MAX_COL], int row, int col, Graph start, Graph end) {
    int distance[MAX_ROW][MAX_COL];
    int visited[MAX_ROW][MAX_COL] = {0};
    Graph path[MAX_ROW][MAX_COL];

    SetInf(distance, row, col);
    distance[start.x][start.y] = 0;

    for (int count = 0; count < row * col - 1; count++) {
        Graph min_Graph;
        int min_Distance = INF;

        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (!visited[i][j] && distance[i][j] < min_Distance) {
                    min_Distance = distance[i][j];
                    min_Graph.x = i;
                    min_Graph.y = j;
                }
            }
        }

        visited[min_Graph.x][min_Graph.y] = 1;

        int distance_x[4] = {-1, 0, 1, 0};
        int distance_y[4] = {0, 1, 0, -1};

        for (int k = 0; k < 4; k++) {
            int new_x = min_Graph.x + distance_x[k];
            int new_y = min_Graph.y + distance_y[k];
            if (valid(new_x, new_y, row, col) && maze[new_x][new_y] != '#' && !visited[new_x][new_y]) {
                if (distance[min_Graph.x][min_Graph.y] + 1 < distance[new_x][new_y]) {
                    distance[new_x][new_y] = distance[min_Graph.x][min_Graph.y] + 1;
                    path[new_x][new_y] = min_Graph;
                }
            }
        }
    }

    if (distance[end.x][end.y] != INF) {
        printf("JALUR TERPENDEK ADALAH: %d\n", distance[end.x][end.y]);
        printf("Langkah-langkah: ");
        printPath_Dijkstra(path, start, end, maze, row, col);
        printf("\n");
    } else {
        printf("Tidak ada jalur dari start ke end.\n");
    }
}

double CalcTime(clock_t start, clock_t finish) {
    return ((double)(finish - start)) / CLOCKS_PER_SEC;
}

void readMazeFile(char *filename, char maze[MAX_ROW][MAX_COL], int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("File tak bisa dibuka\n");
        exit(1);
    }

    char line[MAX_COL];
    *rows = 0;
    *cols = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        if (*cols == 0) {
            *cols = strlen(line);
        }
        strcpy(maze[*rows], line);
        (*rows)++;
    }

    fclose(file);
}