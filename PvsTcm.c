#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <time.h>

#define ROWS 5
#define COLS 5
#define MINES 5

int field[ROWS][COLS];
pthread_mutex_t lock;

void* sweep_row(void* arg) {
    int row = *(int*)arg;
    for (int col = 0; col < COLS; col++) {
        pthread_mutex_lock(&lock);
        if (field[row][col] == 1) {
            printf("Process %d, Thread sweeping row %d: Mine found at (%d,%d)!\n", getpid(), row, row, col);
        } else {
            printf("Process %d, Thread sweeping row %d: Safe at (%d,%d)\n", getpid(), row, row, col);
        }
        pthread_mutex_unlock(&lock);
        usleep(100000);
    }
    return NULL;
}

void place_mines() {
    int placed = 0;
    while (placed < MINES) {
        int r = rand() % ROWS;
        int c = rand() % COLS;
        if (field[r][c] == 0) {
            field[r][c] = 1;
            placed++;
        }
    }
}

int main() {
    srand(time(NULL));
    pthread_mutex_init(&lock, NULL);

    // Place mines only in parent process
    place_mines();

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) { // Child process
        printf("Child process (pid=%d) sweeping field...\n", getpid());
    } else { // Parent process
        printf("Parent process (pid=%d) sweeping field...\n", getpid());
    }

    pthread_t threads[ROWS];
    int rows[ROWS];
    for (int i = 0; i < ROWS; i++) {
        rows[i] = i;
        pthread_create(&threads[i], NULL, sweep_row, &rows[i]);
    }

    for (int i = 0; i < ROWS; i++) {
        pthread_join(threads[i], NULL);
    }

    if (pid > 0) {
        wait(NULL);
        printf("Simulation finished.\n");
    }

    pthread_mutex_destroy(&lock);
    return 0;
}