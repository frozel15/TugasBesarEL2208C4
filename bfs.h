#ifndef BFS_H
#define BFS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define MAX 255

/** Koordinat untuk maze.
 * @param x titik x.
 * @param y titik y.
*/
typedef struct Coords {
    int x;
    int y;
} Coords;

/** Node menyimpan posisi, node sebelumnya, dan branch selanjutnya.
 * @param pos posisi (x,y) node.
 * @param prev parent/node sebelumnya.
 * @param next 4 array selanjutnya (N, S, W, E), tidak berurutan.
*/
typedef struct Node {
    Coords pos;
    struct Node* prev;
    struct Node** next;
} Node;

/** Node queue untuk BFS, dengan data berisi node.
 * @param node Node data.
 * @param next Node queue selanjutnya yang akan dioperasikan.
*/
typedef struct QueueNode {
    Node* node;
    struct QueueNode *next;
} QueueNode;

/** Implementasi queue.
 * @param head head pointer menuju node paling depan dalam queue.
 * @param tail pointer menuju node paling belakang dalam queue.
*/
typedef struct Queue {
    QueueNode *head;
    QueueNode *tail;
} Queue;

/** isQueueEmpty. Cek apakah queue kosong.
* @param queue pointer menuju queue.
* @return queue->head == NULL.
*/
int isQueueEmpty(Queue* queue);

/** enqueue. Tambahkan node ke belakang queue.
*
* @param queue pointer menuju queue.
* @param node node baru untuk ditambahkan ke dalam queue.
*/
void enqueue(Queue* queue, Node* node);

/** Dequeue. Hapus elemen paling depan dari queue, ambil datanya.
* @param queue pointer menuju queue.
* @return queue->head->node bila ada, 0 bila tidak.
*/
QueueNode* dequeue(Queue* queue);

/** Mendapatkan jumlah baris dan kolom dari maze
 * @param filename nama file yang akan dibuka.
 * @param row pointer ke variabel yang memuat data jumlah baris
 * @param column pointer ke variabel yang memuat data jumlah kolom
*/
int getRowColumn(const char *filename, int *row, int *column);

/** Inisialisasi maze column x row.
 * @param maze pointer ke maze.
 * @param row data jumlah baris.
 * @param column data jumlah kolom.
*/
void initializeMaze(int ***maze, int row, int column);

/** Memasukkan data di file ke maze.
 * @param filename nama file yang akan dibuka.
 * @param maze variabel maze.
 * @param row jumlah baris.
 * @param column jumlah kolom.
 * @param start koordinat (x,y) awal
 * @param end koordinat (x,y) akhir
*/
void addToMaze(const char *filename, int **maze, int row, int column, Coords *start, Coords *end);

/** Mengubah variabel next menjadi NULL.
 * @param node Node data.
*/
void setNextNull(Node *node);

/** Cek apakah titik (x,y) pernah dijelajahi sebelumnya.
 * @param node Node data.
 * @param x titik x yang dicek.
 * @param y titik y yang dicek.
*/
int isDescendant(Node *node, int x, int y);

/** Cek apakah traversal ke x dan y valid.
 * @param node Node data.
 * @param maze variabel maze.
 * @param x titik x yang dicek.
 * @param y titik y yang dicek.
 * @param row jumlah baris.
 * @param column jumlah kolom.
 * @return 1 jika semua kondisi memenuhi.
*/
int isValid_BFS(Node *node, int **maze, int x, int y, int row, int column);

/** Membuat node berisikan parent node, 4 next node, posisi x dan y
 * @param node Node data.
 * @param x titik x dari node.
 * @param y titik y dari node.
 * @param i untuk array next.
*/
void makeNode(Node *node, int x, int y, int i);

/** Melepaskan node dan parent node dalam memori, dengan kondisi parent tidak memiliki next node selain node dicek.
 * @param node Node data.
*/
void freeNodefromEnd(Node *node);

/** Algoritma utama BFS.
 * @param queue untuk enqueue node next.
 * @param node Node data.
 * @param maze variabel maze.
 * @param row jumlah baris.
 * @param column jumlah kolom.
*/
void traverse(Queue *queue, Node *node, int **maze, int row, int column);

/** Output path dari awal ke current node.
 * @param node Node data.
*/
void printPath(Node *node);

/** Output maze dan path yang sedang dicek.
 * @param maze Data maze awal.
 * @param row jumlah baris.
 * @param column jumlah kolom.
 * @param node Node data.
*/
void printMaze_BFS(int **maze, int row, int column, Node* node);

/** Menghitung panjang jalur yang dilalui.
 * @param node Node data.
 * @return panjang jalur.
*/
int pathLen(Node *node);

/** Melepas seluruh node dari memori.
 * @param node Head node.
*/
void freeNode(Node *node);

/** Melepas queue dari memori.
 * @param queue variabel queue.
*/
void freeQueue(Queue *queue);

/** Melepas maze dari memori.
 * @param maze variabel maze.
*/
void freeMaze(int **maze, int row);

/** Output path dari current node ke file.
 * @param file pointer ke file yang akan dioperasikan.
 * @param node Node data.
*/
void writePathToFile(FILE *file, Node *node);

/** Output shortest dan longest path, dan write file berisi semua path. 
 * @param queue main queue.
 * @param maze variabel maze.
 * @param row jumlah baris.
 * @param column jumlah kolom.
 * @param end titik akhir.
 * @param mode mode operasi fungsi.
*/
void solveBFS(Queue *queue, int **maze, int row, int column, Coords end, int mode, clock_t start_time);
#endif