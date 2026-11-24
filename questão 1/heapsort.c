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

void heapify(int *vet, int n, int raiz, long *comp, long *trocas) {
    int maior = raiz;
    int esq = 2 * raiz + 1;
    int dir = 2 * raiz + 2;

    if (esq < n) {
        (*comp)++;
        if (vet[esq] > vet[maior]) maior = esq;
    }

    if (dir < n) {
        (*comp)++;
        if (vet[dir] > vet[maior]) maior = dir;
    }

    if (maior != raiz) {
        int aux = vet[raiz];
        vet[raiz] = vet[maior];
        vet[maior] = aux;
        (*trocas)++;
        heapify(vet, n, maior, comp, trocas);
    }
}

void heap_sort(int *vet, int n, long *comp, long *trocas) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(vet, n, i, comp, trocas);
    }
    for (int i = n - 1; i > 0; i--) {
        int aux = vet[0];
        vet[0] = vet[i];
        vet[i] = aux;
        (*trocas)++;
        heapify(vet, i, 0, comp, trocas);
    }
}

int main(void) {
    int vet[MAX];
    int n = carregar_dados("dados_q1.txt", vet, MAX);

    long comp = 0, trocas = 0;

    printf("=== HeapSort ===\n");
    print_vetor("Original", vet, n);

    clock_t t_inicio = clock();
    if (n > 1) {
        heap_sort(vet, n, &comp, &trocas);
    }
    clock_t t_fim = clock();

    double tempo_ms = ((double)(t_fim - t_inicio) / CLOCKS_PER_SEC) * 1000.0;

    print_vetor("Ordenado", vet, n);
    printf("Comparacoes: %ld | Trocas: %ld | Tempo: %.4f ms\n", comp, trocas, tempo_ms);

    return 0;
}