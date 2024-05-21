#include "bfs.h"
#include <unistd.h>

int isQueueEmpty(Queue* queue) {
    return queue->head == NULL;
}

void enqueue(Queue* queue, Node* node) {
    QueueNode *newQueueNode = malloc(sizeof(QueueNode));
    newQueueNode->node = node;
    newQueueNode->next = NULL;
    if (!isQueueEmpty(queue)) {
        queue->tail->next = newQueueNode;
        queue->tail = newQueueNode;
    } else {
        queue->tail = newQueueNode;
        queue->head = newQueueNode;
    }
}

QueueNode* dequeue(Queue* queue) {
    if (isQueueEmpty(queue)) return NULL;
    QueueNode *value = queue->head;
    queue->head = value->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    return value;
}

int getRowColumn(const char *filename, int *row, int *column) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("File gagal dibuka.");
        return 1;
    }
    char line[MAX];
    while (fgets(line, MAX, file)) {
        (*row)++;
        if ((*column) == 0) {
            while(line[*column] != '\n' && line[*column] != '\0') {
                switch (line[*column]) {
                    case 'S':
                        break;
                    case 'E':
                        break;
                    case '#':
                        break;
                    case '.':
                        break;
                    default:
                        printf("Terdapat elemen lain di maze.");
                        fclose(file);
                        return 1;
                }
                (*column)++;
            }
        } else {
            int length = strlen(line)-1;
            if (length != (*column)) {
                if (length == (*column)-1) {
                    if (line[length] == line[(*column)-1]) {
                        switch (line[*column-1]) {
                            case 'S':
                                break;
                            case 'E':
                                break;
                            case '#':
                                break;
                            case '.':
                                break;
                            default:
                                printf("%d %d\n", line[length], line[*column]);
                                printf("Terdapat elemen lain di maze.");
                                fclose(file);
                                return 1;
                        }
                        continue;
                    } else {
                        printf("Maze bukan mxn.");
                        fclose(file);
                        return 1;
                    }
                }
                printf("Maze bukan mxn.");
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
}

void initializeMaze(int ***maze, int row, int column) {
    *maze = malloc(row * sizeof(int*));
    for (int i = 0; i < row; i++) {
        (*maze)[i] = malloc(column * sizeof(int));
        for (int j = 0; j < column; j++) {
            (*maze)[i][j] = 0;
        }
    }
}

void addToMaze(const char *filename, int **maze, int row, int column, Coords *start, Coords *end) {
    FILE *file = fopen(filename, "r");
    char line[MAX];
    int i = 0;
    while (fgets(line, MAX, file)) {
        for (int j = 0; j < column; j++) {
            if (line[j] == '.') {
                maze[i][j] = 0;
            } else if (line[j] == 'S') {
                start->x = j; start->y = i;
                maze[i][j] = -1;
            } else if (line[j] == 'E') {
                end->x = j; end->y = i;
                maze[i][j] = -2;
            } else {
                maze[i][j] = -3;
            }
        }
        i++;
    }
    fclose(file);
}

void setNextNull(Node *node) {
    node->next = malloc(4 * sizeof(Node*));
    for (int i = 0; i < 4; i++) {
        node->next[i] = NULL;
    }
}

int isDescendant(Node *node, int x, int y) {
    Node *temp = node;
    while (temp != NULL) {
        if (temp->pos.x == x && temp->pos.y == y) {
            return 1;
        }
        temp = temp->prev;
    }
    return 0;
}

int isValid_BFS(Node *node, int **maze, int x, int y, int row, int column) {
    return x < column && x >= 0 && y < row && y >= 0 && maze[y][x] != -3 && !isDescendant(node, x, y);
}

void makeNode(Node *node, int x, int y, int i) {
    Node *newNode = malloc(sizeof(Node));
    newNode->pos.x = x; newNode->pos.y = y;
    setNextNull(newNode);
    newNode->prev = node;
    node->next[i] = newNode;
}

void freeNodefromEnd(Node *node) {
    int hasOther = 0, idx = 0;
    if (node->prev == NULL) {
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (node->prev->next[i] == node) {
            idx = i;
        }
        if (node->prev->next[i] != NULL && node->prev->next[i] != node){
            hasOther = 1;
        }
    }
    free(node->prev->next[idx]);
    if (hasOther) {
        return;
    }
    freeNodefromEnd(node->prev);
}

void traverse(Queue *queue, Node *node, int **maze, int row, int column) {
    int deadend = 1;
    if (isValid_BFS(node, maze, node->pos.x-1, node->pos.y, row, column)) {
        makeNode(node, node->pos.x-1, node->pos.y, 0); // Left
    }
    if (isValid_BFS(node, maze, node->pos.x+1, node->pos.y, row, column)) {
        makeNode(node, node->pos.x+1, node->pos.y, 1); // Right
    }
    if (isValid_BFS(node, maze, node->pos.x, node->pos.y-1, row, column)) {
        makeNode(node, node->pos.x, node->pos.y-1, 2); // Up
    }
    if (isValid_BFS(node, maze, node->pos.x, node->pos.y+1, row, column)) {
        makeNode(node, node->pos.x, node->pos.y+1, 3); // Down
    }

    for (int j = 0; j < 4; j++) {
        if (node->next[j] != NULL) {
            deadend = 0;
            enqueue(queue, node->next[j]);
        }
    }
    if (deadend) {
        freeNodefromEnd(node);
    }
}

void printPath(Node *node) {
    if (node == NULL) return;
    printPath(node->prev);
    printf("(%d, %d) -> ", node->pos.x, node->pos.y);
}

void printMaze_BFS(int **maze, int row, int column, Node *path) {
    char displayMaze[row][column + 1];
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            if (maze[i][j] == -3) {
                displayMaze[i][j] = '#';
            } else if (maze[i][j] == -1) {
                displayMaze[i][j] = 'S';
            } else if (maze[i][j] == -2) {
                displayMaze[i][j] = 'E';
            } else {
                displayMaze[i][j] = '.';
            }
        }
        displayMaze[i][column] = '\0';
    }

    int count = 1;
    while (path->prev != NULL) {
        if (displayMaze[path->pos.y][path->pos.x] == '.') {
            count++;
            displayMaze[path->pos.y][path->pos.x] = '*';
        }
        path = path->prev;
    }

    printf("\033[H");
    // system("cls");

    for (int i = 0; i < row; i++) {
        printf("%s\n", displayMaze[i]);
    }
    printf("%d\n", count);

    // Delay agar terlihat proses algoritma, optional
    usleep(10000);
}

int pathLen(Node *node) {
    if (node->prev == NULL) return 0; // If on root
    return 1 + pathLen(node->prev);
}

void freeNode(Node *node) {
    if (node == NULL) return;
    for (int i = 0; i < 4; i++) {
        if (node->next[i] != NULL) {
            freeNode(node->next[i]);
        }
    }
    free(node->next);
    free(node);
}

void freeQueue(Queue *queue) {
    while (!isQueueEmpty(queue)) {
        QueueNode *current = dequeue(queue);
        free(current);
    }
    free(queue);
}

void freeMaze(int **maze, int row) {
    for (int i = 0; i < row; i++) {
        free(maze[i]);
    }
    free(maze);
}

void writePathToFile(FILE *file, Node *node) {
    if (node == NULL) {
        return;
    }
    writePathToFile(file, node->prev);
    fprintf(file, "(%d, %d) -> ", node->pos.x, node->pos.y);
}

void solveBFS(Queue *queue, int **maze, int row, int column, Coords end, int mode) {
    FILE *allPath = fopen("allPath.txt", "w");
    fprintf(allPath, "");
    fclose(allPath);

    allPath = fopen("allPath.txt", "a");
    int shortest = 0, count = 0;
    clock_t start_time, s_time, l_time;
    double time_spent;
    start_time = clock();
    Node *last = NULL;
    if (mode == 1) {
        while (!isQueueEmpty(queue)) {
            QueueNode *current = dequeue(queue);

            if (current->node->pos.x == end.x && current->node->pos.y == end.y) {
                if (shortest == 0) {
                    shortest = 1;
                    s_time = clock();
                    printf("Shortest path |Length = %d| : ", pathLen(current->node));
                    printPath(current->node);
                    printf("\n");
                    time_spent = (double)(s_time - start_time)/CLOCKS_PER_SEC;
                    printf("Time to find shortest path: %f\n", time_spent);
                }
                last = current->node;
                fprintf(allPath, "Length = %d | ", pathLen(current->node));
                writePathToFile(allPath, current->node);
                fprintf(allPath, "\n");
                count++;
            } else {
                traverse(queue, current->node, maze, row, column);
            }
            
            free(current);
        }
    } else if (mode == 2) {
        while (!isQueueEmpty(queue)) {
            QueueNode *current = dequeue(queue);

            if (current->node->pos.x == end.x && current->node->pos.y == end.y) {
                if (shortest == 0) {
                    shortest = 1;
                    s_time = clock();
                    printf("Shortest path |Length = %d| : ", pathLen(current->node));
                    printPath(current->node);
                    printf("\n");
                    time_spent = (double)(s_time - start_time)/CLOCKS_PER_SEC;
                    printf("Time to find shortest path: %f\n", time_spent);
                } else {
                    printf("Length = %d | ", pathLen(current->node));
                    printPath(current->node);
                    printf("\n");
                }
                last = current->node;
                count++;
            } else {
                traverse(queue, current->node, maze, row, column);
            }
            
            free(current);
        }
    } else {
        while (!isQueueEmpty(queue)) {
            QueueNode *current = dequeue(queue);
            printMaze_BFS(maze, row, column, current->node);

            if (current->node->pos.x == end.x && current->node->pos.y == end.y) {
                if (shortest == 0) {
                    shortest = 1;
                    s_time = clock();
                    time_spent = (double)(s_time - start_time)/CLOCKS_PER_SEC;
                    printf("Time to find shortest path: %f\n", time_spent);
                }
                last = current->node;
                count++;
            } else {
                traverse(queue, current->node, maze, row, column);
            }
            
            free(current);
        }
    }
    
    l_time = clock();

    if (last != NULL) {
        printf("Longest path |Length = %d| : ", pathLen(last));
        printPath(last);
        printf("\n");
    } else {
        printf("No path was found.\n");
    }
    fclose(allPath);
    time_spent = (double)(l_time - start_time)/CLOCKS_PER_SEC;
    printf("Time to find longest path: %f\n", time_spent);
    printf("Amount of path: %d\n", count);

    freeQueue(queue);
    freeMaze(maze, row);
}