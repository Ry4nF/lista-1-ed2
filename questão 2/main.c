#include <stdio.h>
#include <time.h>
#include "fila_prioridade.h"

int main(void) {
    FilaPrioridade *fp = fp_criar(50);
    if (!fp) {
        printf("Erro ao inicializar a fila de prioridade.\n");
        return 1;
    }

    printf("Fila de Prioridade com Heap Maximo\n\n");

    // Simulacao com dados de teste: {ID, Prioridade}
    int dados_teste[][2] = {
        {1, 1},
        {2, 3},
        {3, 2},
        {4, 1},
        {5, 3}
    };
    int n = sizeof(dados_teste) / sizeof(dados_teste[0]);

    clock_t inicio = clock();

    printf("1. Inserindo elementos:\n");
    for (int i = 0; i < n; i++) {
        fp_inserir(fp, dados_teste[i][0], dados_teste[i][1]);
        printf(" -> Inserido: ID %d | Prioridade %d\n", dados_teste[i][0], dados_teste[i][1]);
    }

    printf("\n2. Chamando elementos (atendimento por prioridade/chegada):\n");
    Dado item;
    while (fp_atender(fp, &item)) {
        printf(" -> Chamado : ID %d | Prioridade %d (Ordem de chegada: %d)\n",
               item.id, item.prioridade, item.ordem_chegada);
    }

    clock_t fim = clock();
    double tempo_ms = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;

    MetricasHeap m = fp_obter_metricas(fp);
    printf("\n--- Metricas de Desempenho do Heap ---\n");
    printf("Comparacoes internas : %ld\n", m.comparacoes);
    printf("Trocas no array      : %ld\n", m.trocas);
    printf("Tempo total medido   : %.4f ms\n", tempo_ms);

    fp_destruir(fp);
    return 0;
}