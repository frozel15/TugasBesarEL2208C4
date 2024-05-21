#ifndef ASTAR_H
#define ASTAR_H

typedef struct
{
    int x, y;
} Point_Astar;

// Deklarasi fungsi shortestPath dan longestPath
void findshortestPath_Astar(char **maze, int baris, int kolom, Point_Astar start, Point_Astar end);
void findLongestPath_Astar(char **maze, int baris, int kolom, Point_Astar start, Point_Astar end);
int bacaFile(char *namaFile, char ***matriks, int *baris, int *kolom);

#endif
