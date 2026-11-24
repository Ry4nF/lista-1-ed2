#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000

void print_vetor(const char *msg, int *vet, int n) {
    printf("%-10s: ", msg);
    for (int i = 0; i < n; i++) {
        printf("%d ", vet[i]);
    }
    printf("\n");
}

int carregar_dados(const char *caminho, int *vet, int limite) {
    FILE *f = fopen(caminho, "r");
    if (!f) {
        printf("Erro ao abrir %s\n", caminho);
        exit(1);
    }
    int total = 0;
    while (total < limite && fscanf(f, "%d", &vet[total]) == 1) {
        total++;
    }
    fclose(f);
    return total;
}

int particiona(int *vet, int inicio, int fim, long *comp, long *trocas) {
    int pivo = vet[fim];
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        (*comp)++;
        if (vet[j] < pivo) {
            i++;
            int aux = vet[i];
            vet[i] = vet[j];
            vet[j] = aux;
            (*trocas)++;
        }
    }

    int aux = vet[i + 1];
    vet[i + 1] = vet[fim];
    vet[fim] = aux;
    (*trocas)++;

    return i + 1;
}

void quick_sort(int *vet, int inicio, int fim, long *comp, long *trocas) {
    if (inicio < fim) {
        int p = particiona(vet, inicio, fim, comp, trocas);
        quick_sort(vet, inicio, p - 1, comp, trocas);
        quick_sort(vet, p + 1, fim, comp, trocas);
    }
}

int main(void) {
    int vet[MAX];
    int n = carregar_dados("dados_q1.txt", vet, MAX);

    long comp = 0, trocas = 0;

    printf("=== QuickSort ===\n");
    print_vetor("Original", vet, n);

    clock_t t_inicio = clock();
    if (n > 1) {
        quick_sort(vet, 0, n - 1, &comp, &trocas);
    }
    clock_t t_fim = clock();

    double tempo_ms = ((double)(t_fim - t_inicio) / CLOCKS_PER_SEC) * 1000.0;

    print_vetor("Ordenado", vet, n);
    printf("Comparacoes: %ld | Trocas: %ld | Tempo: %.4f ms\n", comp, trocas, tempo_ms);

    return 0;
}