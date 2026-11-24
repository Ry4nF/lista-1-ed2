#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_vetor(const char *msg, int *vet, int n) {
    printf("%-10s: ", msg);
    for (int i = 0; i < n; i++) {
        printf("%d ", vet[i]);
    }
    printf("\n");
}

void counting_sort(int *vet, int n, int exp, long *contagens, long *movimentos) {
    int *saida = (int *)malloc(n * sizeof(int));
    if (!saida) {
        printf("Falha na alocacao de memoria\n");
        exit(1);
    }

    int cont[10] = {0};

    for (int i = 0; i < n; i++) {
        int digito = (vet[i] / exp) % 10;
        cont[digito]++;
        (*contagens)++;
    }

    for (int d = 1; d < 10; d++) {
        cont[d] += cont[d - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int digito = (vet[i] / exp) % 10;
        saida[--cont[digito]] = vet[i];
        (*movimentos)++;
    }

    for (int i = 0; i < n; i++) {
        vet[i] = saida[i];
        (*movimentos)++;
    }

    free(saida);
}


void radix_sort(int *vet, int n, long *contagens, long *movimentos) {
    if (n <= 1) return;

    int max = vet[0];
    for (int i = 1; i < n; i++) {
        if (vet[i] > max) max = vet[i];
    }

    for (int exp = 1; max / exp > 0; exp *= 10) {
        counting_sort(vet, n, exp, contagens, movimentos);
    }
}

int main(void) {

    int vet[] = {329, 457, 657, 839, 436, 720, 355, 314, 623, 213, 232, 645, 587, 485, 156, 478};
    int n = sizeof(vet) / sizeof(vet[0]);

    long contagens = 0;
    long movimentos = 0;

    printf("=== RadixSort com CountingSort ===\n\n");
    print_vetor("Original", vet, n);

    clock_t inicio = clock();
    radix_sort(vet, n, &contagens, &movimentos);
    clock_t fim = clock();

    double tempo_ms = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;

    print_vetor("Ordenado", vet, n);

    printf("\n--- Metricas de Execucao ---\n");
    printf("Itens no vetor           : %d\n", n);
    printf("Contagens realizadas     : %ld\n", contagens);
    printf("Movimentacoes auxiliares : %ld\n", movimentos);
    printf("Tempo total de execucao  : %.4f ms\n", tempo_ms);

    return 0;
}