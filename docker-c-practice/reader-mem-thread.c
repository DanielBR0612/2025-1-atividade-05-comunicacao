#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>

#define SHM_SIZE 1024

// Função idêntica à lógica original, mas adaptada para thread
void* ler_memoria_compartilhada(void* arg) {
    int shmid;
    key_t key = 1234;  // Mesma chave usada no escritor
    char *shm;

    // Obtém o segmento de memória compartilhada
    shmid = shmget(key, SHM_SIZE, 0666);
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

    // Lê a mensagem da memória compartilhada
    printf("Leitor: Mensagem lida: \"%s\"\n", shm);

    // Sinaliza para o escritor que terminou (modificando o primeiro byte)
    *shm = '*';

    // Libera a memória compartilhada
    shmdt(shm);

    printf("Leitor: Finalizado.\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;

    // Cria a thread que executa a leitura
    if (pthread_create(&thread, NULL, ler_memoria_compartilhada, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    // Espera a thread terminar
    pthread_join(thread, NULL);

    return 0;
}