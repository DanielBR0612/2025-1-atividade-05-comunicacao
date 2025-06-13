#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#define FILENAME "comunicacao.txt"

// Função idêntica à lógica original, mas adaptada para thread
void* escrever_arquivo(void* arg) {
    FILE *file;
    const char *message = "Olá, comunicação via arquivo!\n";

    file = fopen(FILENAME, "w");
    if (file == NULL) {
        perror("fopen");
        pthread_exit(NULL);
    }

    fwrite(message, 1, strlen(message), file);
    fclose(file);

    printf("Escritor: Mensagem escrita no arquivo.\n");
    printf("Escritor: Aguardando leitura...\n");

    while (access(FILENAME, F_OK) == 0) {
        sleep(1);
    }

    printf("Escritor: Arquivo lido e removido. Finalizado.\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;

    // Cria a thread que executa a escrita
    if (pthread_create(&thread, NULL, escrever_arquivo, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    // Espera a thread terminar (como o programa original faria sequencialmente)
    pthread_join(thread, NULL);

    return 0;
}