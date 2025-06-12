#include <unistd.h>  
#include <stdio.h>  

int main() {  
    printf("Processo PAI (PID: %d)\n", getpid());  
    fork();  // Cria um filho  
    printf("Quem sou eu? PID: %d\n", getpid());  
    sleep(1);  // Pausa para ver a saída  
    return 0;  
}  