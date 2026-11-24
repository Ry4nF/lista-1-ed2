#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ordenacao_externa.h"


typedef struct {
    FILE *f;
    int pos;
    int max;
    int *dados;
} Buffer;

struct ordenadorExterno {
    int tam_ram;
    char prefixo[16];
    MetricasExterna metricas;
};

//quicksort
static int particiona(int *v, int inicio, int fim) {
    int pivo = v[fim];
    int i = inicio - 1;
    for (int j = inicio; j < fim; j++) {
        if (v[j] < pivo) {
            i++;
            int t = v[i];
            v[i] = v[j];
            v[j] = t;
        }
    }
    int t = v[i + 1];
    v[i + 1] = v[fim];
    v[fim] = t;
    return i + 1;
}

static void quick_sort(int *v, int inicio, int fim) {
    if (inicio < fim) {
        int p = particiona(v, inicio, fim);
        quick_sort(v, inicio, p - 1);
        quick_sort(v, p + 1, fim);
    }
}

static void salvar_bloco(const char *nome, int *v, int tam, MetricasExterna *m) {
    FILE *f = fopen(nome, "a");
    if (!f) {
        printf("Erro ao salvar arquivo: %s\n", nome);
        exit(1);
    }
    for (int i = 0; i < tam; i++) {
        fprintf(f, "%d\n", v[i]);
        m->escritas_io++;
    }
    fclose(f);
}

static void preencher_buffer(Buffer *b, int limite, MetricasExterna *m) {
    if (b->f == NULL) return;
    b->pos = 0;
    b->max = 0;
    for (int i = 0; i < limite; i++) {
        if (fscanf(b->f, "%d", &b->dados[b->max]) == 1) {
            b->max++;
            m->leituras_io++;
        } else {
            fclose(b->f);
            b->f = NULL;
            break;
        }
    }
}

static int achar_menor(Buffer *bufs, int k, int t, int *menor, MetricasExterna *m) {
    int idx = -1;
    for (int i = 0; i < k; i++) {
        if (bufs[i].pos < bufs[i].max) {
            if (idx == -1 || bufs[i].dados[bufs[i].pos] < bufs[idx].dados[bufs[idx].pos]) {
                idx = i;
            }
        }
    }
    if (idx == -1) return 0;

    *menor = bufs[idx].dados[bufs[idx].pos];
    bufs[idx].pos++;
    if (bufs[idx].pos == bufs[idx].max) {
        preencher_buffer(&bufs[idx], t, m);
    }
    return 1;
}

static int criar_blocos(OrdenadorExterno *oe, const char *caminho) {
    int *ram = (int *)malloc(oe->tam_ram * sizeof(int));
    if (!ram) exit(1);

    FILE *f = fopen(caminho, "r");
    if (!f) {
        free(ram);
        return 0;
    }

    char temp_nome[32];
    int k = 0, qtd = 0;

    while (fscanf(f, "%d", &ram[qtd]) == 1) {
        oe->metricas.leituras_io++;
        qtd++;
        if (qtd == oe->tam_ram) {
            k++;
            snprintf(temp_nome, sizeof(temp_nome), "%s%d.txt", oe->prefixo, k);
            quick_sort(ram, 0, qtd - 1);
            salvar_bloco(temp_nome, ram, qtd, &oe->metricas);
            qtd = 0;
        }
    }

    if (qtd > 0) {
        k++;
        snprintf(temp_nome, sizeof(temp_nome), "%s%d.txt", oe->prefixo, k);
        quick_sort(ram, 0, qtd - 1);
        salvar_bloco(temp_nome, ram, qtd, &oe->metricas);
    }

    fclose(f);
    free(ram);
    return k;
}

static void intercalar(OrdenadorExterno *oe, const char *destino, int k, int t) {
    char temp_nome[32];
    int *saida = (int *)malloc(t * sizeof(int));
    Buffer *bufs = (Buffer *)malloc(k * sizeof(Buffer));

    if (!saida || !bufs) exit(1);

    for (int i = 0; i < k; i++) {
        snprintf(temp_nome, sizeof(temp_nome), "%s%d.txt", oe->prefixo, i + 1);
        bufs[i].f = fopen(temp_nome, "r");
        bufs[i].dados = (int *)malloc(t * sizeof(int));
        preencher_buffer(&bufs[i], t, &oe->metricas);
    }

    int menor, qtd_saida = 0;
    while (achar_menor(bufs, k, t, &menor, &oe->metricas)) {
        saida[qtd_saida++] = menor;
        if (qtd_saida == t) {
            salvar_bloco(destino, saida, t, &oe->metricas);
            qtd_saida = 0;
        }
    }

    if (qtd_saida > 0) {
        salvar_bloco(destino, saida, qtd_saida, &oe->metricas);
    }

    for (int i = 0; i < k; i++) free(bufs[i].dados);
    free(bufs);
    free(saida);
}

OrdenadorExterno* oe_criar(int capacidade_ram, const char *prefixo_temp) {
    if (capacidade_ram <= 0) capacidade_ram = 100;

    OrdenadorExterno *oe = (OrdenadorExterno *)malloc(sizeof(OrdenadorExterno));
    if (!oe) return NULL;

    oe->tam_ram = capacidade_ram;
    strncpy(oe->prefixo, prefixo_temp ? prefixo_temp : "Temp", sizeof(oe->prefixo) - 1);
    oe->prefixo[sizeof(oe->prefixo) - 1] = '\0';

    oe->metricas.blocos_k = 0;
    oe->metricas.leituras_io = 0;
    oe->metricas.escritas_io = 0;
    oe->metricas.tempo_ms = 0.0;
    return oe;
}

void oe_destruir(OrdenadorExterno *oe) {
    if (oe) free(oe);
}

bool oe_gerar_teste(const char *caminho, int qtd) {
    FILE *f = fopen(caminho, "w");
    if (!f) return false;

    srand(42);
    for (int i = 0; i < qtd; i++) {
        fprintf(f, "%d\n", rand() % 100000);
    }
    fclose(f);
    return true;
}

bool oe_ordenar(OrdenadorExterno *oe, const char *caminho) {
    if (!oe || !caminho) return false;

    clock_t t1 = clock();

    int k = criar_blocos(oe, caminho);
    oe->metricas.blocos_k = k;
    if (k == 0) return false;

    int t = oe->tam_ram / (k + 1);
    if (t == 0) t = 1;

    remove(caminho);
    intercalar(oe, caminho, k, t);

    char temp_nome[32];
    for (int i = 0; i < k; i++) {
        snprintf(temp_nome, sizeof(temp_nome), "%s%d.txt", oe->prefixo, i + 1);
        remove(temp_nome);
    }

    clock_t t2 = clock();
    oe->metricas.tempo_ms = ((double)(t2 - t1) / CLOCKS_PER_SEC) * 1000.0;
    return true;
}

bool oe_verificar(const char *caminho, int *total) {
    FILE *f = fopen(caminho, "r");
    if (!f) return false;

    int anterior, atual, lidos = 0;
    bool ok = true;

    if (fscanf(f, "%d", &anterior) == 1) {
        lidos = 1;
        while (fscanf(f, "%d", &atual) == 1) {
            lidos++;
            if (atual < anterior) {
                ok = false;
                break;
            }
            anterior = atual;
        }
    }
    fclose(f);
    if (total) *total = lidos;
    return ok;
}

MetricasExterna oe_obter_metricas(const OrdenadorExterno *oe) {
    if (!oe) {
        MetricasExterna vazia = {0, 0, 0, 0.0};
        return vazia;
    }
    return oe->metricas;
}