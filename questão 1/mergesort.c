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

void merge(int *vet, int inicio, int meio, int fim, long *comp, long *trocas) {
    int tam = fim - inicio + 1;
    int *temp = (int *)malloc(tam * sizeof(int));
    if (!temp) {
        printf("Falha no malloc\n");
        exit(1);
    }

    int i = inicio, j = meio + 1, k = 0;

    while (i <= meio && j <= fim) {
        (*comp)++;
        if (vet[i] <= vet[j]) {
            temp[k++] = vet[i++];
        } else {
            temp[k++] = vet[j++];
        }
        (*trocas)++;
    }

    while (i <= meio) {
        temp[k++] = vet[i++];
        (*trocas)++;
    }
    while (j <= fim) {
        temp[k++] = vet[j++];
        (*trocas)++;
    }

    for (i = inicio, k = 0; i <= fim; i++, k++) {
        vet[i] = temp[k];
        (*trocas)++;
    }

    free(temp);
}

void merge_sort(int *vet, int inicio, int fim, long *comp, long *trocas) {
    if (inicio < fim) {
        int meio = (inicio + fim) / 2;
        merge_sort(vet, inicio, meio, comp, trocas); 
        merge_sort(vet, meio + 1, fim, comp, trocas);
        merge(vet, inicio, meio, fim, comp, trocas);
    }
}

int main(void) {
    int vet[MAX];
    int n = carregar_dados("dados_q1.txt", vet, MAX);

    long comp = 0, trocas = 0;

    printf("=== MergeSort ===\n");
    print_vetor("Original", vet, n);

    clock_t t_inicio = clock();
    if (n > 1) {
        merge_sort(vet, 0, n - 1, &comp, &trocas);
    }
    clock_t t_fim = clock();

    double tempo_ms = ((double)(t_fim - t_inicio) / CLOCKS_PER_SEC) * 1000.0;

    print_vetor("Ordenado", vet, n);
    printf("Comparacoes: %ld | Movimentacoes: %ld | Tempo: %.4f ms\n", comp, trocas, tempo_ms);

    return 0;
}