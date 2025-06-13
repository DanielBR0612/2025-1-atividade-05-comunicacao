#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>

#define SHM_SIZE 1024  // Tamanho da memória compartilhada

// Função idêntica à lógica original, mas adaptada para thread
void* escrever_memoria_compartilhada(void* arg) {
    int shmid;
    key_t key = 1234;  // Chave única para a memória compartilhada
    char *shm;

    // Cria um segmento de memória compartilhada
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        pthread_exit(NULL);
    }

    // Anexa o segmento ao espaço de endereçamento do processo
    shm = shmat(shmid, NULL, 0);
    if (shm == (char *)-1) {
        perror("shmat");
        pthread_exit(NULL);
    }

    // Escreve na memória compartilhada
    strcpy(shm, "Olá, Memória Compartilhada!");

    printf("Escritor: Mensagem escrita na memória compartilhada.\n");
    printf("Escritor: Aguardando leitura...\n");

    // Espera até que o leitor sinalize que terminou
    while (*shm != '*') {
        sleep(1);
    }

    // Libera a memória compartilhada
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);  // Remove o segmento

    printf("Escritor: Memória compartilhada liberada.\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;

    // Cria a thread que executa a escrita
    if (pthread_create(&thread, NULL, escrever_memoria_compartilhada, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    // Espera a thread terminar
    pthread_join(thread, NULL);

    return 0;
}