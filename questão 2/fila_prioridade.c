#include <stdio.h>
#include <stdlib.h>
#include "fila_prioridade.h"

typedef struct {
    Dado dado;
    int serial;
} ItemHeap;

struct filaPrioridade {
    ItemHeap *heap;
    int total;
    int capacidade;
    int contador_chegada;
    MetricasHeap metricas;
};

// Formula adaptada do livro do Forouzan (Cap. 9): prioridade domina e chegada desempata (FIFO)
static int calcular_serial(int prioridade, int ordem) {
    return (prioridade * 1000) + (999 - (ordem % 1000));
}

static void trocar_itens(ItemHeap *a, ItemHeap *b) {
    ItemHeap tmp = *a;
    *a = *b;
    *b = tmp;
}

static void subir_no_heap(FilaPrioridade *fp, int idx) {
    if (idx <= 0) return;
    int pai = (idx - 1) / 2;

    fp->metricas.comparacoes++;
    if (fp->heap[idx].serial > fp->heap[pai].serial) {
        trocar_itens(&fp->heap[idx], &fp->heap[pai]);
        fp->metricas.trocas++;
        subir_no_heap(fp, pai);
    }
}

static void descer_no_heap(FilaPrioridade *fp, int idx) {
    int maior = idx;
    int esq = 2 * idx + 1;
    int dir = 2 * idx + 2;

    if (esq < fp->total) {
        fp->metricas.comparacoes++;
        if (fp->heap[esq].serial > fp->heap[maior].serial) {
            maior = esq;
        }
    }

    if (dir < fp->total) {
        fp->metricas.comparacoes++;
        if (fp->heap[dir].serial > fp->heap[maior].serial) {
            maior = dir;
        }
    }

    if (maior != idx) {
        trocar_itens(&fp->heap[idx], &fp->heap[maior]);
        fp->metricas.trocas++;
        descer_no_heap(fp, maior);
    }
}

FilaPrioridade* fp_criar(int capacidade) {
    if (capacidade <= 0) capacidade = 100;

    FilaPrioridade *fp = (FilaPrioridade *)malloc(sizeof(FilaPrioridade));
    if (!fp) return NULL;

    fp->heap = (ItemHeap *)malloc(capacidade * sizeof(ItemHeap));
    if (!fp->heap) {
        free(fp);
        return NULL;
    }

    fp->total = 0;
    fp->capacidade = capacidade;
    fp->contador_chegada = 0;
    fp->metricas.comparacoes = 0;
    fp->metricas.trocas = 0;

    return fp;
}

void fp_destruir(FilaPrioridade *fp) {
    if (fp) {
        free(fp->heap);
        free(fp);
    }
}

bool fp_inserir(FilaPrioridade *fp, int id, int prioridade) {
    if (!fp || fp->total >= fp->capacidade) return false;

    int chegada = fp->contador_chegada++;
    ItemHeap novo = {
        .dado = { id, prioridade, chegada },
        .serial = calcular_serial(prioridade, chegada)
    };

    fp->heap[fp->total] = novo;
    fp->total++;
    subir_no_heap(fp, fp->total - 1);

    return true;
}

bool fp_atender(FilaPrioridade *fp, Dado *atendido) {
    if (!fp || fp->total == 0) return false;

    if (atendido) {
        *atendido = fp->heap[0].dado;
    }

    fp->total--;
    fp->heap[0] = fp->heap[fp->total];
    descer_no_heap(fp, 0);

    return true;
}

int fp_tamanho(const FilaPrioridade *fp) {
    return fp ? fp->total : 0;
}

MetricasHeap fp_obter_metricas(const FilaPrioridade *fp) {
    if (!fp) {
        MetricasHeap vazia = {0, 0};
        return vazia;
    }
    return fp->metricas;
}