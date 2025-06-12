#include <stdio.h>  
#include <unistd.h>  
#include <pthread.h>  
#include <stdlib.h>  
#include <time.h>  
#include <string.h>  
#include <sys/wait.h>

// Cores ANSI para o terminal  
#define RED "\033[1;31m"  
#define GREEN "\033[1;32m"  
#define YELLOW "\033[1;33m"  
#define BLUE "\033[1;34m"  
#define RESET "\033[0m"  

// Dados compartilhados (threads)  
typedef struct {  
    int sector_id;  
    int sensor_id;  
    int threat_level;  
} SensorData;  

pthread_mutex_t lock;  

// Mapa ASCII dos setores  
char battlefield_map[10][50]; // 10 linhas x 50 colunas  

void init_map() {  
    for (int i = 0; i < 10; i++) {  
        snprintf(battlefield_map[i], 50, "|...........................|...........................|");  
    }  
}  

void print_map() {  
    printf("\n\n=== MAPA DO CAMPO DE BATALHA ===\n");  
    for (int i = 0; i < 10; i++) {  
        printf("%s\n", battlefield_map[i]);  
    }  
    printf("================================\n");  
}  

// Atualiza o mapa com ameaças  
void update_map(int sector, int sensor, int threat) {  
    int pos = (sector - 1) * 25 + sensor * 5;  
    char marker = (threat > 70) ? 'X' : 'o';  
    battlefield_map[5][pos] = marker;  // Linha central do setor  
}  

// Função da thread (análise de sensor)  
void* analyze_sensor(void* arg) {  
    SensorData* data = (SensorData*)arg;  
    pthread_mutex_lock(&lock);  

    update_map(data->sector_id, data->sensor_id, data->threat_level);  

    printf("[SENSOR %d-%d] Nível de ameaça: ", data->sector_id, data->sensor_id);  
    if (data->threat_level > 70) {  
        printf(RED "%d%% (ALERTA CRÍTICO!)" RESET, data->threat_level);  
    } else if (data->threat_level > 40) {  
        printf(YELLOW "%d%% (ATENÇÃO)" RESET, data->threat_level);  
    } else {  
        printf(GREEN "%d%% (SEGURO)" RESET, data->threat_level);  
    }  
    printf("\n");  

    pthread_mutex_unlock(&lock);  
    return NULL;  
}  

// Função do processo (análise de setor)  
void analyze_sector(int sector_id) {  
    printf(BLUE "\n[PROCESSO] Iniciando análise do Setor %d..." RESET "\n", sector_id);  
    pthread_t threads[3];  
    SensorData sensors[3];  

    for (int i = 0; i < 3; i++) {  
        sensors[i].sector_id = sector_id;  
        sensors[i].sensor_id = i;  
        sensors[i].threat_level = rand() % 100;  
        pthread_create(&threads[i], NULL, analyze_sensor, &sensors[i]);  
    }  

    for (int i = 0; i < 3; i++) {  
        pthread_join(threads[i], NULL);  
    }  

    print_map();  
}  

int main() {  
    srand(time(NULL));  
    pthread_mutex_init(&lock, NULL);  
    init_map();  

    printf(GREEN "=== BATTLEFIELD ANALYZER (C - Processos/Threads) ===" RESET "\n");  
    print_map();  

    pid_t pid = fork();  

    if (pid == 0) {  
        analyze_sector(1);  // Setor 1 (processo filho)  
    } else {  
        analyze_sector(2);  // Setor 2 (processo pai)  
        waitpid(pid, NULL, 0);  // Substitua wait(NULL) por isso  
    }  

    pthread_mutex_destroy(&lock);  
    return 0;  
}  