#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <time.h>

#define LINHAS 5
#define COLUNAS 5
#define MINAS 5

int campo[LINHAS][COLUNAS];
pthread_mutex_t lock;

// Função executada por cada thread para varrer uma linha
void* varrer_linha(void* arg) {
    int linha = *(int*)arg;
    for (int coluna = 0; coluna < COLUNAS; coluna++) {
        pthread_mutex_lock(&lock);
        if (campo[linha][coluna] == 1) {
            printf("Processo %d, Thread varrendo linha %d: Mina encontrada em (%d,%d)!\n", getpid(), linha, linha, coluna);
        } else {
            printf("Processo %d, Thread varrendo linha %d: Seguro em (%d,%d)\n", getpid(), linha, linha, coluna);
        }
        pthread_mutex_unlock(&lock);
        usleep(100000); // Espera 0,1 segundo para simular trabalho
    }
    return NULL;
}

// Função para posicionar as minas aleatoriamente no campo
void posicionar_minas() {
    int colocadas = 0;
    while (colocadas < MINAS) {
        int l = rand() % LINHAS;
        int c = rand() % COLUNAS;
        if (campo[l][c] == 0) {
            campo[l][c] = 1;
            colocadas++;
        }
    }
}

int main() {
    srand(time(NULL));
    pthread_mutex_init(&lock, NULL);

    // Posiciona as minas apenas no processo pai
    posicionar_minas();

    pid_t pid = fork();

    if (pid < 0) {
        perror("Erro ao criar processo");
        exit(1);
    }

    if (pid == 0) { // Processo filho
        printf("Processo filho (pid=%d) iniciando varredura do campo...\n", getpid());
    } else { // Processo pai
        printf("Processo pai (pid=%d) iniciando varredura do campo...\n", getpid());
    }

    pthread_t threads[LINHAS];
    int linhas[LINHAS];
    for (int i = 0; i < LINHAS; i++) {
        linhas[i] = i;
        pthread_create(&threads[i], NULL, varrer_linha, &linhas[i]);
    }

    for (int i = 0; i < LINHAS; i++) {
        pthread_join(threads[i], NULL);
    }

    if (pid > 0) {
        wait(NULL);
        printf("Simulação finalizada.\n");
    }

    pthread_mutex_destroy(&lock);
    return 0;
}