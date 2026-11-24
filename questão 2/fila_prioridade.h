#ifndef FILA_PRIORIDADE_H
#define FILA_PRIORIDADE_H

#include <stdbool.h>

typedef struct {
    int id;
    int prioridade;
    int ordem_chegada;
} Dado;

typedef struct {
    long comparacoes;
    long trocas;
} MetricasHeap;

typedef struct filaPrioridade FilaPrioridade;

FilaPrioridade* fp_criar(int capacidade);
void fp_destruir(FilaPrioridade *fp);

bool fp_inserir(FilaPrioridade *fp, int id, int prioridade);
bool fp_atender(FilaPrioridade *fp, Dado *atendido);
int fp_tamanho(const FilaPrioridade *fp);
MetricasHeap fp_obter_metricas(const FilaPrioridade *fp);

#endif