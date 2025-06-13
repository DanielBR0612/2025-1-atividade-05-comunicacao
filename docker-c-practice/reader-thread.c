#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define FILENAME "comunicacao.txt"
#define TEMPNAME "comunicacao.lida"

// Copia **exata** da função original, mas adaptada para rodar em uma thread
void* ler_arquivo(void* arg) {
    FILE *file;
    char buffer[1024];

    file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("fopen");
        pthread_exit(NULL);
    }

    fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    printf("Leitor: Mensagem lida:\n%s", buffer);

    if (rename(FILENAME, TEMPNAME) != 0) {
        perror("rename");
        pthread_exit(NULL);
    }

    printf("Leitor: Arquivo renomeado para %s. Finalizado.\n", TEMPNAME);
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;

    // Cria a thread que executa a função `ler_arquivo` (sem argumentos extras)
    if (pthread_create(&thread, NULL, ler_arquivo, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    // Espera a thread terminar (equivalente ao comportamento sequencial original)
    pthread_join(thread, NULL);

    return 0;
}