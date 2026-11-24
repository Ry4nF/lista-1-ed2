#ifndef ORDENACAO_EXTERNA_H
#define ORDENACAO_EXTERNA_H

#include <stdbool.h>

typedef struct {
    int blocos_k;
    long leituras_io;
    long escritas_io;
    double tempo_ms;
} MetricasExterna;

typedef struct ordenadorExterno OrdenadorExterno;

OrdenadorExterno* oe_criar(int capacidade_ram, const char *prefixo_temp);
void oe_destruir(OrdenadorExterno *oe);

bool oe_gerar_teste(const char *caminho, int qtd);
bool oe_ordenar(OrdenadorExterno *oe, const char *caminho);
bool oe_verificar(const char *caminho, int *total);
MetricasExterna oe_obter_metricas(const OrdenadorExterno *oe);

#endif