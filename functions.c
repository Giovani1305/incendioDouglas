#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N 30

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N 30

typedef struct {
    char floresta[N][N];
    pthread_mutex_t mutex[N][N];
    pthread_cond_t cond[N][N];
} MonitorFloresta;

void inicializa_floresta(MonitorFloresta *monitor) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            monitor->floresta[i][j] = '-';
            pthread_mutex_init(&monitor->mutex[i][j], NULL);
            pthread_cond_init(&monitor->cond[i][j], NULL);
        }
    }
}

void trava_celula(MonitorFloresta *monitor, int linha, int coluna) {
    pthread_mutex_lock(&monitor->mutex[linha][coluna]);
}

void destrava_celula(MonitorFloresta *monitor, int linha, int coluna) {
    pthread_mutex_unlock(&monitor->mutex[linha][coluna]);
}

void espera_fogo(MonitorFloresta *monitor, int linha, int coluna) {
    pthread_cond_wait(&monitor->cond[linha][coluna], &monitor->mutex[linha][coluna]);
}

void sinaliza_fogo(MonitorFloresta *monitor, int linha, int coluna) {
    pthread_cond_signal(&monitor->cond[linha][coluna]);
}

void printa_floresta(MonitorFloresta *monitor) {
    printf("\nEstado atual da floresta:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%c ", monitor->floresta[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void notifica_vizinhos(MonitorFloresta *monitor, int linha, int coluna) {
    for (int i = linha - 1; i <= linha + 1; i++) {
        for (int j = coluna - 1; j <= coluna + 1; j++) {
            if (i >= 0 && i < N && j >= 0 && j < N && (i != linha || j != coluna)) {
                trava_celula(monitor, i, j);
                if (monitor->floresta[i][j] == '-') {
                    monitor->floresta[i][j] = '!';
                    printf("Célula [%d][%d] notificada do incêndio!\n", i, j);
                }
                destrava_celula(monitor, i, j);
            }
        }
    }
}

void combater_fogo(MonitorFloresta *monitor, int linha, int coluna) {
    if (monitor->floresta[linha][coluna] == '@') {
        monitor->floresta[linha][coluna] = '/'; 
    }
}

void *sensor_thread(void *arg) {
    MonitorFloresta *monitor = (MonitorFloresta *)arg;
    int linha = rand() % N;
    int coluna = rand() % N;

    while (1) {
        trava_celula(monitor, linha, coluna);

        if (monitor->floresta[linha][coluna] == '@') {
            printf("Sensor [%d][%d] detectou fogo!\n", linha, coluna);
            notifica_vizinhos(monitor, linha, coluna);
            printa_floresta(monitor);
        }

        destrava_celula(monitor, linha, coluna);
        usleep(100000);
    }

    return NULL;
}

void *gerador_fogo(void *arg) {
    MonitorFloresta *monitor = (MonitorFloresta *)arg;

    while (1) {
        int linha = rand() % N;
        int coluna = rand() % N;

        trava_celula(monitor, linha, coluna);

        if (monitor->floresta[linha][coluna] == '-') {
            monitor->floresta[linha][coluna] = '@';
            printf("Incêndio gerado em [%d][%d]\n", linha, coluna);
            printa_floresta(monitor);
        }

        destrava_celula(monitor, linha, coluna);
        sleep(3);
    }

    return NULL;
}

void *central_de_controle(void *arg) {
    MonitorFloresta *monitor = (MonitorFloresta *)arg;

    while (1) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if ((i == 0 || i == N - 1 || j == 0 || j == N - 1) && monitor->floresta[i][j] == '!') {
                    printf("Central de controle detectou fogo na célula borda [%d][%d] e iniciará combate!\n", i, j);
                    combater_fogo(monitor, i, j);
                }
            }
        }
        usleep(100000);
    }

    return NULL;
}

void inicializa_floresta(MonitorFloresta *monitor) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            monitor->floresta[i][j] = '-';
            pthread_mutex_init(&monitor->mutex[i][j], NULL);
            pthread_cond_init(&monitor->cond[i][j], NULL);
        }
    }
}

void trava_celula(MonitorFloresta *monitor, int linha, int coluna) {
    pthread_mutex_lock(&monitor->mutex[linha][coluna]);
}

void destrava_celula(MonitorFloresta *monitor, int linha, int coluna) {
    pthread_mutex_unlock(&monitor->mutex[linha][coluna]);
}

void espera_fogo(MonitorFloresta *monitor, int linha, int coluna) {
    pthread_cond_wait(&monitor->cond[linha][coluna], &monitor->mutex[linha][coluna]);
}

void sinaliza_fogo(MonitorFloresta *monitor, int linha, int coluna) {
    pthread_cond_signal(&monitor->cond[linha][coluna]);
}

void printa_floresta(MonitorFloresta *monitor) {
    printf("\nEstado atual da floresta:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%c ", monitor->floresta[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void notifica_vizinhos(MonitorFloresta *monitor, int linha, int coluna) {
    for (int i = linha - 1; i <= linha + 1; i++) {
        for (int j = coluna - 1; j <= coluna + 1; j++) {
            if (i >= 0 && i < N && j >= 0 && j < N && (i != linha || j != coluna)) {
                trava_celula(monitor, i, j);
                if (monitor->floresta[i][j] == '-') {
                    monitor->floresta[i][j] = '!';
                    printf("Célula [%d][%d] notificada do incêndio!\n", i, j);
                }
                destrava_celula(monitor, i, j);
            }
        }
    }
}

void combater_fogo(MonitorFloresta *monitor, int linha, int coluna) {
    if (monitor->floresta[linha][coluna] == '@') {
        monitor->floresta[linha][coluna] = '/'; 
    }
}

void *sensor_thread(void *arg) {
    MonitorFloresta *monitor = (MonitorFloresta *)arg;
    int linha = rand() % N;
    int coluna = rand() % N;

    while (1) {
        trava_celula(monitor, linha, coluna);

        if (monitor->floresta[linha][coluna] == '@') {
            printf("Sensor [%d][%d] detectou fogo!\n", linha, coluna);
            notifica_vizinhos(monitor, linha, coluna);
            printa_floresta(monitor);
        }

        destrava_celula(monitor, linha, coluna);
        usleep(100000);
    }

    return NULL;
}

void *gerador_fogo(void *arg) {
    MonitorFloresta *monitor = (MonitorFloresta *)arg;

    while (1) {
        int linha = rand() % N;
        int coluna = rand() % N;

        trava_celula(monitor, linha, coluna);

        if (monitor->floresta[linha][coluna] == '-') {
            monitor->floresta[linha][coluna] = '@';
            printf("Incêndio gerado em [%d][%d]\n", linha, coluna);
            printa_floresta(monitor);
        }

        destrava_celula(monitor, linha, coluna);
        sleep(3);
    }

    return NULL;
}

void *central_de_controle(void *arg) {
    MonitorFloresta *monitor = (MonitorFloresta *)arg;

    while (1) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if ((i == 0 || i == N - 1 || j == 0 || j == N - 1) && monitor->floresta[i][j] == '!') {
                    printf("Central de controle detectou fogo na célula borda [%d][%d] e iniciará combate!\n", i, j);
                    combater_fogo(monitor, i, j);
                }
            }
        }
        usleep(100000);
    }

    return NULL;
}
