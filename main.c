#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "color.h"
#include "AStar.h"
#include "Backtracking.h"
#include "bfs.h"
#include "dfs.h"
#include "Dijkstra.h"
#include "greedy.h"

int main_astar(char *namaFile, clock_t start_shortest) {
    // char namaFile[100];
    // printf("Masukkan nama file: ");
    // scanf("%s", namaFile);

    char **matriks;
    int baris, kolom;

    // Membuka file
    int berhasil = bacaFile(namaFile, &matriks, &baris, &kolom);
    while (!berhasil)
    {
        printf("File tidak bisa dibuka\n");
        printf("Nama file yang ingin dibaca: ");
        scanf("%s", namaFile);
        berhasil = bacaFile(namaFile, &matriks, &baris, &kolom);
    }

    // Mencari posisi start dan end
    Point_Astar start = {-1, -1}; // Inisialisasi start dan end dengan nilai yang tidak mungkin muncul di labirin
    Point_Astar end = {-1, -1};

    for (int y = 0; y < baris; y++)
    {
        for (int x = 0; x < kolom; x++)
        {
            if (matriks[y][x] == 'S')
            {
                start.x = x;
                start.y = y;
            }
            else if (matriks[y][x] == 'E')
            {
                end.x = x;
                end.y = y;
            }
        }
    }

    // Periksa apakah start dan end telah ditemukan
    if (start.x == -1 || start.y == -1 || end.x == -1 || end.y == -1)
    {
        printf("Titik awal atau akhir tidak ditemukan dalam labirin.\n");
        return 0;
    }

    // Fungsi mencari shortest path dengan A-Star
    findshortestPath_Astar(matriks, baris, kolom, start, end, start_shortest);

    // Fungsi mencari longest path dengan A-Star
    findLongestPath_Astar(matriks, baris, kolom, start, end, start_shortest);

    // Membebaskan memori matriks
    for (int i = 0; i < baris; i++)
    {
        free(matriks[i]);
    }
    free(matriks);
}

int main_backtracking(char *fileMaze, clock_t start) {
    // char fileMaze[100];
    // printf("Input maze file name: ");
    // scanf("%s", fileMaze);

    printf("\nAnalyzing maze...");

    FILE *file = fopen("Maze Analysis.txt", "w+");
    if (!file) {
        printf("\nFile doesn't exist.\n");
        exit(1);
    }
    fprintf(file, "All possible paths from start to end:\n");

    readMaze(fileMaze);

    Path currentPath = {{0}, 0};
    memset(visited, 0, sizeof(visited));
    findPaths(startY, startX, &currentPath, file);
    clock_t end = clock();
    double timeSolve = (double)(end - start) / CLOCKS_PER_SEC;

    fprintf(file, "\nTotal number of paths: %d\n", pathCount);
    fprintf(file, "Time to find all paths: %.6f second(s)\n", timeSolve);

    int shortestLength, longestLength;
    char shortestPath[1024], longestPath[1024];
    findPeakPaths(file, &shortestLength, shortestPath, &longestLength, longestPath);

    fprintf(file, "\nShortest path (%d steps)\n%s", shortestLength-1, shortestPath);
    fprintf(file, "\nLongest path (%d steps)\n%s", longestLength-1, longestPath);

    fclose(file);

    printf("\nMaze is solved in 'Maze Analysis.txt'.");

    return 0;
}

int main_bfs(char *namafile, int mode, clock_t start_time) {
    // char namafile[10];
    // printf("Masukkan nama file: ");
    // scanf("%s", namafile);

    // int mode;
    // do {
    //     printf("Pilih mode:\n");
    //     printf("1. Write ke file\n");
    //     printf("2. Print ke terminal\n");
    //     printf("3. Ilustrasi BFS\n");
    //     scanf("%d", &mode);
    // } while (mode != 1 && mode != 2 && mode != 3);
    
    clock_t s_time = clock();
    int row = 0, column = 0;
    if (getRowColumn(namafile, &row, &column)) {
        return 1;
    }

    int **maze;
    Coords start, end;
    initializeMaze(&maze, row, column); 
    addToMaze(namafile, maze, row, column, &start, &end);

    Node *head = malloc(sizeof(Node));
    setNextNull(head);
    head->prev = NULL;
    head->pos.x = start.x; head->pos.y = start.y;

    Queue *queue = malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    enqueue(queue, head);
    system("cls");

    solveBFS(queue, maze, row, column, end, mode, start_time);
    clock_t e_time = clock();
    return 0;
}

int main_dfs(char *txt, clock_t start) {
    FILE *allPath = fopen("allPath.txt", "w");
    fprintf(allPath, "");
    fclose(allPath);

    char labirin[MAX][MAX];
    int baris, kolom;
    // char txt[30];
    resetMap(labirin);
    // printf("Masukkan File Txt Struktur Maze : ");
    // scanf("%s", txt );
    // clock_t start = clock();
    if (!bacaLabirin(txt, labirin, &baris, &kolom)) {
        return 1; // Exit jika file tidak bisa dibuka
    }

    int tempxend;
    int tempyend;
    for (int i = 0; i < baris; i++) {
        for (int j = 0; j < kolom; j++) {
            if (labirin[i][j] == -4) { //mark
                findRoute(labirin, j, i, 1, baris, kolom);
                break;
            }
        }
    }

    for (int i = 0; i < baris; i++) {
        for (int j = 0; j < kolom; j++) {
            if (labirin[i][j] == -1) { //mark
                tempxend= i;
                tempyend = j;
                break;
            }
        }
    }

    int path[MAX][MAX];
    printf("All the possible path is : \n");
    resetMap(labirin);
    bacaLabirin(txt, labirin, &baris, &kolom);
    findLongestPath(labirin, tempxend, tempyend, baris, kolom, path, 0);

    printf("The quantity of possible path is: %d\n", pathquantity);

    for (int i = 0; i < baris; i++) {
        for (int j = 0; j < kolom; j++) {
            if (labirin[i][j] == -4) { //mark
                findRoute(labirin, j, i, 1, baris, kolom);
                break;
            }
        }
    }

    if (pathquantity > 0){ // jika tidak ada path maka tidak print shortest, dan longest
        printf("The shortest path is : \n ");
        tracing (labirin, tempxend, tempyend, baris, kolom, 0);

        resetMap(labirin);
        bacaLabirin(txt, labirin, &baris, &kolom);
        
        printf("The longest path is:\n");
        printf("Length = %d | ", longestPathLength);
        for (int i = 0; i < longestPathLength; i++) {
            if (i < longestPathLength -1){
                printf("(%d, %d) -> ", longestPath[i][0], longestPath[i][1]);
            } else {
                printf("(%d, %d)", longestPath[i][0], longestPath[i][1]);
            }
        }
        printf("\n");
    }
    
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken for DFS process: %f seconds\n", time_spent);

    return 0;
}

int main_dijkstra(char *filename, clock_t short_start) {
    clock_t short_end;
    double time_short;

    // char filename[MAX_ROW];
    char maze[MAX_ROW][MAX_COL];
    int rows, cols;

    // printf("Masukkan nama file: ");
    // scanf("%s", filename);

    readMazeFile(filename, maze, &rows, &cols);

    // printf("Maze sebelum dijalankan algoritma:\n");
    // for (int i = 0; i < rows; i++) {
    //     printf("%s\n", maze[i]);
    // }
    // printf("\n");

    Graph start = {-1, -1}, end = {-1, -1};
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == 'S') {
                start.x = i;
                start.y = j;
            }
            if (maze[i][j] == 'E') {
                end.x = i;
                end.y = j;
            }
        }
    }

    if (start.x == -1 || end.x == -1) {
        printf("Start or end point not found in the maze.\n");
        return -1;
    }

    Dijkstra(maze, rows, cols, start, end);
    short_end = clock();

    time_short = CalcTime(short_start, short_end);
    printf("Waktu yang diperlukan: %f detik\n", time_short);

    printf("Maze dengan jalur terpendek:\n");
    for (int i = 0; i < rows; i++) {
        printf("%s\n", maze[i]);
    }

    return 0;
}

int main_greedy(char *filename, clock_t waktu_awal) {
    FILE *file;
    // char filename[100];
    char maze[MAX][MAX];
    int row = 0, col = 0;
    Point_Greedy start, end;
    Path_Greedy paths[MAX];
    int pathCount = 0;
    clock_t waktu_akhir;
    double waktu;

    // printf("Masukkan nama file: ");
    // scanf("%s", filename);
    waktu_awal = clock();

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("File tidak dapat dibuka.\n");
        return 1;
    }

    while (fgets(maze[row], MAX, file)) {
        maze[row][strcspn(maze[row], "\n")] = '\0';
        for (int j = 0; j < strlen(maze[row]); j++) {
            if (maze[row][j] == 'S') {
                start.x = row;
                start.y = j;
            }
            if (maze[row][j] == 'E') {
                end.x = row;
                end.y = j;
            }
        }
        row++;
    }
    fclose(file);

    col = strlen(maze[0]);

    printf("Maze:\n");
    printMaze_Greedy(maze, row, col);
    printf("\n");

    Path_Greedy currentPath;
    solveMaze(maze, row, col, &currentPath, start, end);

    if (currentPath.length > 1) {
        paths[pathCount++] = currentPath;
    }

    printf("All possible paths from start to end:\n");
    for (int i = 0; i < pathCount; i++) {
        printf("Path %d: ", i + 1);
        printPath_Greedy(paths[i]);
    }

    if (pathCount > 0) {
        Path_Greedy shortestPath = paths[0];
        Path_Greedy longestPath = paths[0];

        for (int i = 1; i < pathCount; i++) {
            if (paths[i].length < shortestPath.length) {
                shortestPath = paths[i];
            }
            if (paths[i].length > longestPath.length) {
                longestPath = paths[i];
            }
        }

        printf("Total number of paths: %d\n", pathCount);
        printf("Shortest path from start to end:\n");
        printPath_Greedy(shortestPath);
        printf("Longest path from start to end:\n");
        printPath_Greedy(longestPath);
    } else {
        printf("No paths found from start to end.\n");
    }
    waktu_akhir = clock();
    waktu = ((double)(waktu_akhir - waktu_awal)) / CLOCKS_PER_SEC;
    printf("waktu= %f detik\n", waktu);

    return 0;
}

int main() {
    system("cls");
    printf(BRED "Algoritma Maze Solver Kelompok C4\n" BWHT);

    char namafile[MAX];
    printf(GRN "Nama file: " reset);
    scanf("%s", namafile);

    int menu;
    system("cls");
    printf(BRED "Algoritma Maze Solver Kelompok C4\n" BWHT);
    printf("1. Djikstra\n");
    printf("2. DFS\n");
    printf("3. BFS\n");
    printf("4. A-star\n");
    printf("5. Backtracking\n");
    printf("6. Greedy\n" reset);
    scanf("%d", &menu);

    int mode;
    clock_t start_time, end_time;
    double time_spent;
    system("cls");
    switch (menu) {
        case 1:
            start_time = clock();
            main_dijkstra(namafile, start_time);
            break;
        case 2:
            start_time = clock();
            main_dfs(namafile, start_time);
            break;
        case 3:
            do {
                printf(BRED "Algoritma Maze Solver Kelompok C4\n" BWHT);
                printf(GRN "Pilih mode:\n" BWHT);
                printf("1. Write ke file\n");
                printf("2. Print ke terminal\n");
                printf("3. Ilustrasi BFS\n" reset);
                scanf("%d", &mode);
                if (!(mode == 1 || mode == 2 || mode == 3)) {
                    printf("Mode tidak valid. Harap ulangi.\n");
                    Sleep(1000);
                    system("cls");
                }
            } while (mode != 1 && mode != 2 && mode != 3);
            start_time = clock();
            main_bfs(namafile, mode, start_time);
            break;
        case 4:
            start_time = clock();
            main_astar(namafile, start_time);
            break;
        case 5:
            start_time = clock();
            main_backtracking(namafile, start_time);
            break;
        case 6:
            start_time = clock();
            main_greedy(namafile, start_time);
            break;
    }
    end_time = clock();
    time_spent = (double)(end_time - start_time)/CLOCKS_PER_SEC;
    printf("Time spent: %f", time_spent);
}