#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#define SIZE_Y 25
#define SIZE_X 80
#define STEP 10000
#define DEAD 0
#define ALIVE 1

void AllocateMemory(int ***matrix, int y, int x);
void FreeMemory(int ** matrix, int n);
void DrawField(int ** matrix, int y, int x);
int CountNeighbors(int ** matrix, int y, int x);
void CountLive(int ** matrix, int ** new_matrix, int y, int x);
void CopyArray(int ** matrix, int ** new_matrix, int y, int x);
void FillArray(int ** matrix);
void FillZeros(int ** matrix, int y, int x);
char GetInput(useconds_t * speed);
void FillRandomly(int ** matrix, int y, int x);

int main(void) {
    int ** Field = NULL;
    int ** NextField = NULL;
    AllocateMemory(&Field, SIZE_Y, SIZE_X);
    AllocateMemory(&NextField, SIZE_Y, SIZE_X);
    FillZeros(Field, SIZE_Y, SIZE_X);
    FillArray(Field);
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, 1);
    scrollok(stdscr, 1);
    useconds_t speed = 50000;
    char key = 0;
    while (key != '.') {
        key = GetInput(&speed);
        DrawField(Field, SIZE_Y, SIZE_X);
        CountLive(Field, NextField, SIZE_Y, SIZE_X);
        CopyArray(NextField, Field, SIZE_Y, SIZE_X);
        usleep(speed);
    }
    FreeMemory(Field, SIZE_Y);
    FreeMemory(NextField, SIZE_Y);
    refresh();
    endwin();
}

void AllocateMemory(int ***matrix, int y, int x) {
    (*matrix) = (int **)malloc(y * sizeof(int*));
    for (int i = 0; i < y; i++) {
        if (*matrix != NULL)
            (*matrix)[i] = (int*)malloc(x * sizeof(int));
    }
}

void FreeMemory(int ** matrix, int n) {
    for (int i = 0; i < n; i++) {
        if (matrix[i] != NULL)
            free(matrix[i]);
    }
    if (matrix != NULL)
        free(matrix);
}

void DrawField(int ** matrix, int y, int x) {
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            if (matrix[i][j] == 1)
                printw("%c", '*');
            else
                printw("%c", ' ');
        }
        printw("\n");
    }
}

void CountLive(int ** matrix, int ** new_matrix, int y, int x) {
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            int State = matrix[i][j];
            int Neighbors = CountNeighbors(matrix, i, j);
            if (State == DEAD && Neighbors == 3) {
                new_matrix[i][j] = ALIVE;
            } else if (State == ALIVE && (Neighbors < 2 || Neighbors > 3)) {
                new_matrix[i][j] = DEAD;
            } else {
                new_matrix[i][j] = State;
            }
        }
    }
}

int CountNeighbors(int ** matrix, int y, int x) {
    int sum = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int x_1 = (x + i + SIZE_X) % SIZE_X;
            int y_1 = (y + j + SIZE_Y) % SIZE_Y;
            sum += matrix[y_1][x_1];
        }
    }

    sum -= matrix[y][x];
    return sum;
}

void CopyArray(int ** matrix, int ** new_matrix, int y, int x) {
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            new_matrix[i][j] = matrix[i][j];
        }
    }
}

void FillArray(int ** matrix) {
    int i = 0;
    int j = 0;
    char c = 0;
    while ((c = getchar()) != EOF) {
        matrix[i][j] = c - 48;
        ++j;
        if (c == '\n') {
            ++i;
            j = 0;
        }
    }
}

void FillZeros(int ** matrix, int y, int x) {
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++){
            matrix[i][j] = 0;
        }
    }
}

char GetInput(useconds_t * speed) {
    char key = getch();
    if (key != ERR) {
        if (key == 'a' || key == 'A') {
            *speed += STEP;
        }
        if (key == 'z'|| key == 'Z') {
            *speed -= STEP;
        }
    }
    return key;
}
