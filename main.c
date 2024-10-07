#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define N 30

typedef struct {
    char floresta[N][N];
    pthread_mutex_t mutex[N][N];
    pthread_cond_t cond[N][N];
} MonitorFloresta;

void inicializa_floresta(MonitorFloresta *monitor);
void trava_celula(MonitorFloresta *monitor, int linha, int coluna);
void destrava_celula(MonitorFloresta *monitor, int linha, int coluna);
void espera_fogo(MonitorFloresta *monitor, int linha, int coluna);
void sinaliza_fogo(MonitorFloresta *monitor, int linha, int coluna);
void notifica_vizinhos(MonitorFloresta *monitor, int linha, int coluna);
void printa_floresta(MonitorFloresta *monitor);
void *sensor_thread(void *arg);
void *gerador_fogo(void *arg);
void *central_de_controle(void *arg);
void combater_fogo(MonitorFloresta *monitor, int linha, int coluna);

int main() {
    MonitorFloresta monitor;
    pthread_t sensores[N * N], thread_fogo, thread_central;

    inicializa_floresta(&monitor);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            pthread_create(&sensores[i * N + j], NULL, sensor_thread, (void *)&monitor);
        }
    }

    pthread_create(&thread_fogo, NULL, gerador_fogo, (void *)&monitor);
    pthread_create(&thread_central, NULL, central_de_controle, (void *)&monitor);

    for (int i = 0; i < N * N; i++) {
        pthread_join(sensores[i], NULL);
    }

    pthread_join(thread_fogo, NULL);
    pthread_join(thread_central, NULL);

    return 0;
}
