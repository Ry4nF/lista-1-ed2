#include <stdio.h>
#include "ordenacao_externa.h"

#define TAM_RAM 100
#define ARQ_DADOS "dados.txt"
#define TOTAL_REGISTROS 1000

int main(void) {
    printf("=== Ordenacao Externa por Intercalacao (TAD) ===\n\n");

    OrdenadorExterno *oe = oe_criar(TAM_RAM, "Temp");
    if (!oe) {
        printf("Erro ao inicializar o ordenador externo.\n");
        return 1;
    }

    printf("1. Gerando %d registros no arquivo '%s'...\n", TOTAL_REGISTROS, ARQ_DADOS);
    oe_gerar_teste(ARQ_DADOS, TOTAL_REGISTROS);

    printf("2. Executando Quick Sort Externo (Memoria RAM: %d itens)...\n", TAM_RAM);
    oe_ordenar(oe, ARQ_DADOS);

    printf("3. Verificando integridade do arquivo ordenado...\n");
    int lidos = 0;
    bool ok = oe_verificar(ARQ_DADOS, &lidos);

    MetricasExterna m = oe_obter_metricas(oe);

    printf("\nDesempenho:\n");
    printf("Total de registros      : %d\n", lidos);
    printf("Blocos temporarios (K)  : %d\n", m.blocos_k);
    printf("Leituras no disco       : %ld\n", m.leituras_io);
    printf("Escritas no disco       : %ld\n", m.escritas_io);
    printf("Tempo total medido      : %.4f ms\n", m.tempo_ms);
    printf("Validacao final         : %s\n", ok ? "ORDENADO COM SUCESSO" : "ERRO DE ORDENACAO");

    oe_destruir(oe);
    return 0;
}