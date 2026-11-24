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

void shell_sort(int *vet, int n, long *comp, long *trocas) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int aux = vet[i];
            int j = i;
            (*trocas)++; 
            while (j >= gap) {
                (*comp)++;
                if (vet[j - gap] > aux) {
                    vet[j] = vet[j - gap];
                    (*trocas)++;
                    j -= gap;
                } else {
                    break;
                }
            }
            vet[j] = aux;
            (*trocas)++;
        }
    }
}

int main(void) {
    int vet[MAX];
    int n = carregar_dados("dados_q1.txt", vet, MAX);

    long comp = 0, trocas = 0;

    printf("=== ShellSort ===\n");
    print_vetor("Original", vet, n);

    clock_t t_inicio = clock();
    shell_sort(vet, n, &comp, &trocas);
    clock_t t_fim = clock();

    double tempo_ms = ((double)(t_fim - t_inicio) / CLOCKS_PER_SEC) * 1000.0;

    print_vetor("Ordenado", vet, n);
    printf("Comparacoes: %ld | Movimentacoes: %ld | Tempo: %.4f ms\n", comp, trocas, tempo_ms);

    return 0;
}