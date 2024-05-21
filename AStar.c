#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "AStar.h"

typedef struct Node
{
    Point_Astar point;
    double f, g, h;
    struct Node *parent;
} Node;

// Fungsi untuk memeriksa apakah node valid
int isValid(Point_Astar pt, int baris, int kolom)
{
    return (pt.x >= 0) && (pt.x < kolom) && (pt.y >= 0) && (pt.y < baris);
}

// Fungsi untuk memeriksa apakah suatu node adalah tujuan
int isDestination(Point_Astar pt, Point_Astar dest)
{
    return (pt.x == dest.x) && (pt.y == dest.y);
}

// Fungsi untuk menghitung jarak Manhattan antara dua node
double manhattanDistance(Point_Astar pt, Point_Astar dest)
{
    return abs(pt.x - dest.x) + abs(pt.y - dest.y);
}

// Fungsi untuk membuat node baru
Node *createNode(Point_Astar pt, double g, double h, Node *parent)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->point = pt;
    newNode->g = g;
    newNode->h = h;
    newNode->f = g + h;
    newNode->parent = parent;
    return newNode;
}

// Fungsi untuk mencetak labirin dan jalur
void printMaze(char **maze, int baris, int kolom, Point_Astar path[], int path_length)
{
    for (int y = 0; y < baris; y++)
    {
        for (int x = 0; x < kolom; x++)
        {
            int isPath = 0;
            for (int i = 0; i < path_length; i++)
            {
                if (path[i].x == x && path[i].y == y)
                {
                    isPath = 1;
                    break;
                }
            }
            if (isPath)
            {
                printf("x");
            }
            else
            {
                printf("%c", maze[y][x]);
            }
        }
        printf("\n");
    }
}

// Fungsi untuk mencari jalur terpendek menggunakan algoritma A-Star
void findshortestPath_Astar(char **maze, int baris, int kolom, Point_Astar start, Point_Astar end, clock_t startshortest)
{
    clock_t endshortest;
    double cpu_time_used_shortest;

    startshortest = clock();
    // Call the function you want to measure here

    int closedList[baris][kolom];
    memset(closedList, 0, sizeof(closedList));

    Node *openList[baris * kolom];
    int openListSize = 0;

    Node *startNode = createNode(start, 0.0, manhattanDistance(start, end), NULL);
    openList[openListSize++] = startNode;

    int dirX[] = {-1, 1, 0, 0};
    int dirY[] = {0, 0, -1, 1};

    Node *currentNode = NULL;

    while (openListSize > 0)
    {
        int currentIndex = 0;
        for (int i = 1; i < openListSize; i++)
        {
            if (openList[i]->f < openList[currentIndex]->f)
            {
                currentIndex = i;
            }
        }

        currentNode = openList[currentIndex];

        if (isDestination(currentNode->point, end))
        {
            break;
        }

        openList[currentIndex] = openList[--openListSize];
        closedList[currentNode->point.y][currentNode->point.x] = 1;

        for (int i = 0; i < 4; i++)
        {
            Point_Astar neighborPoint = {currentNode->point.x + dirX[i], currentNode->point.y + dirY[i]};

            if (isValid(neighborPoint, baris, kolom) && maze[neighborPoint.y][neighborPoint.x] != '#' && closedList[neighborPoint.y][neighborPoint.x] == 0)
            {
                double gNew = currentNode->g + 1.0;
                double hNew = manhattanDistance(neighborPoint, end);
                double fNew = gNew + hNew;

                int foundInOpenList = 0;
                for (int j = 0; j < openListSize; j++)
                {
                    if (openList[j]->point.x == neighborPoint.x && openList[j]->point.y == neighborPoint.y)
                    {
                        foundInOpenList = 1;
                        if (openList[j]->g > gNew)
                        {
                            openList[j]->g = gNew;
                            openList[j]->f = fNew;
                            openList[j]->parent = currentNode;
                        }
                        break;
                    }
                }

                if (!foundInOpenList)
                {
                    Node *neighborNode = createNode(neighborPoint, gNew, hNew, currentNode);
                    openList[openListSize++] = neighborNode;
                }
            }
        }
    }

    endshortest = clock();

    cpu_time_used_shortest = ((double)(endshortest - startshortest)) / CLOCKS_PER_SEC;
    printf("\nWaktu : %.100f detik", cpu_time_used_shortest);

    printf("\nJalur terpendek : \n");
    if (currentNode && isDestination(currentNode->point, end))
    {
        int path_length = 0;
        Point_Astar path[baris * kolom];
        while (currentNode)
        {
            path[path_length++] = currentNode->point;
            currentNode = currentNode->parent;
        }

        // Print semua matriks
        printMaze(maze, baris, kolom, path, path_length);

        // Print jalur dengan posisi
        printf("Jalur terpendek : \n");
        for (int i = path_length - 1; i >= 0; i--)
        {
            printf("(%d, %d)", path[i].x, path[i].y);
            if (i > 0)
            {
                printf(" -> ");
            }
        }
        printf("\n");
    }
    else
    {
        printf("Jalur tidak ditemukan!\n");
    }

    for (int i = 0; i < openListSize; i++)
    {
        free(openList[i]);
    }
}

// Fungsi untuk mencari jalur terjauh menggunakan algoritma A-Star
void findLongestPath_Astar(char **maze, int baris, int kolom, Point_Astar start, Point_Astar end, clock_t startlongest)
{
    clock_t endlongest;
    double cpu_time_used_longest;

    startlongest = clock();
    // Call the function you want to measure here

    int closedList[baris][kolom];
    memset(closedList, 0, sizeof(closedList));

    Node *openList[baris * kolom];
    int openListSize = 0;

    Node *startNode = createNode(start, 0.0, manhattanDistance(start, end), NULL);
    openList[openListSize++] = startNode;

    int dirX[] = {-1, 1, 0, 0};
    int dirY[] = {0, 0, -1, 1};

    Node *currentNode = NULL;

    while (openListSize > 0)
    {
        int currentIndex = 0;
        for (int i = 1; i < openListSize; i++)
        {
            if (openList[i]->f > openList[currentIndex]->f)
            {
                currentIndex = i;
            }
        }

        currentNode = openList[currentIndex];

        if (isDestination(currentNode->point, end))
        {
            break;
        }

        openList[currentIndex] = openList[--openListSize];
        closedList[currentNode->point.y][currentNode->point.x] = 1;

        for (int i = 0; i < 4; i++)
        {
            Point_Astar neighborPoint = {currentNode->point.x + dirX[i], currentNode->point.y + dirY[i]};

            if (isValid(neighborPoint, baris, kolom) && maze[neighborPoint.y][neighborPoint.x] != '#' && closedList[neighborPoint.y][neighborPoint.x] == 0)
            {
                double gNew = currentNode->g + 1.0;
                double hNew = manhattanDistance(neighborPoint, end);
                double fNew = gNew + hNew;

                int foundInOpenList = 0;
                for (int j = 0; j < openListSize; j++)
                {
                    if (openList[j]->point.x == neighborPoint.x && openList[j]->point.y == neighborPoint.y)
                    {
                        foundInOpenList = 1;
                        if (openList[j]->g < gNew)
                        {
                            openList[j]->g = gNew;
                            openList[j]->f = fNew;
                            openList[j]->parent = currentNode;
                        }
                        break;
                    }
                }

                if (!foundInOpenList)
                {
                    Node *neighborNode = createNode(neighborPoint, gNew, hNew, currentNode);
                    openList[openListSize++] = neighborNode;
                }
            }
        }
    }

    endlongest = clock();

    cpu_time_used_longest = ((double)(endlongest - startlongest)) / CLOCKS_PER_SEC;
    printf("\nWaktu : %.100f detik", cpu_time_used_longest);

    printf("\nJalur terjauh : \n");
    if (currentNode && isDestination(currentNode->point, end))
    {
        int path_length = 0;
        Point_Astar path[baris * kolom];
        while (currentNode)
        {
            path[path_length++] = currentNode->point;
            currentNode = currentNode->parent;
        }

        // Print matriks akhir
        printMaze(maze, baris, kolom, path, path_length);

        // Print jalur dengan posisi
        printf("Jalur terjauh : ");
        for (int i = path_length - 1; i >= 0; i--)
        {
            printf("(%d, %d)", path[i].x, path[i].y);
            if (i > 0)
            {
                printf(" -> ");
            }
        }
        printf("\n");
    }
    else
    {
        printf("Jalur tidak ditemukan!\n");
    }

    for (int i = 0; i < openListSize; i++)
    {
        free(openList[i]);
    }
}

int bacaFile(char *namaFile, char ***matriks, int *baris, int *kolom)
{
    // Membuak file dan jika file tidak dapat dibuka
    FILE *file = fopen(namaFile, "r");
    if (file == NULL)
    {
        printf("Gagal membuka file %s\n", namaFile);
        return 0;
    }

    // Menghitung besar ukuran matriks
    int p = 0, l = 0;
    char c;
    while ((c = fgetc(file)) != EOF)
    {
        if (c != '\n')
        {
            p++;
        }
        else
        {
            l++;
        }
    }
    *kolom = p / l;
    *baris = l;

    // Jika matriks tidak ditemukan
    *matriks = (char **)malloc(*baris * sizeof(char *));
    if (*matriks == NULL)
    {
        printf("Gagal mengalokasikan memori\n");
        return 0;
    }
    for (int i = 0; i < *baris; i++)
    {
        (*matriks)[i] = (char *)malloc(*kolom * sizeof(char));
        if ((*matriks)[i] == NULL)
        {
            printf("Gagal mengalokasikan memori\n");
            return 0;
        }
    }

    rewind(file);

    // Membaca file .txt dan diinput menjadi matriks
    for (int i = 0; i < *baris; i++)
    {
        for (int j = 0; j < *kolom; j++)
        {
            do
            {
                c = fgetc(file);
            } while (c == '\n');
            (*matriks)[i][j] = c;
        }
    }

    fclose(file);
    return 1;
}