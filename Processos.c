#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    printf("Processo pai (PID: %d) iniciando.\n", getpid());

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Falha no fork.\n");
        return 1;
    } else if (pid == 0) {
        // Processo filho
        printf("Processo filho (PID: %d) criado. "
               "Meu pai é (PID: %d).\n", 
               getpid(), getppid());
        sleep(2); // Simula algum trabalho
        printf("Processo filho (PID: %d) terminando.\n", 
               getpid());
        exit(0);
    } else {
        // Processo pai
        printf("Processo pai (PID: %d) criou o filho "
               "(PID: %d).\n", getpid(), pid);
        wait(NULL); // Espera o processo filho terminar
        printf("Processo pai (PID: %d) detectou que o "
               "filho terminou. Terminando também.\n", 
               getpid());
    }

    return 0;
}