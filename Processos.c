#include <stdio.h>
#include <unistd.h>
int main() {
    pid_t pid = fork();
    if (pid == 0) {
        printf("Processo filho\n");
    } else {
        printf("Processo pai\n");
    }
    return 0;
}