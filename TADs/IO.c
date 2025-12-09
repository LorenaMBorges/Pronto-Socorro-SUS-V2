#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "IO.h"
#include "AVL.h"
#include "historico.h"
#include "filaPrioridade.h"

// Função auxiliar do SAVE
static bool salvar_string(FILE* arquivo, const char* texto) {
    int tamanho = 0;
    if (texto != NULL) {
        tamanho = (int) strlen(texto);
    }

    if (fwrite(&tamanho, sizeof(int), 1, arquivo) != 1) {
        return false;
    }

    if (tamanho > 0 && fwrite(texto, sizeof(char), tamanho, arquivo) != (size_t)tamanho) {
        return false;
    }

    return true;
}

// Função auxiliar do LOAD
static bool carregar_string(FILE* arquivo, char** texto) {
    int tamanho = 0;
    if (fread(&tamanho, sizeof(int), 1, arquivo) != 1) {
        return false;
    }
    if (tamanho < 0) {
        return false;
    }

    char* buffer = NULL;
    size_t bytes = (size_t) tamanho;

    if (bytes > 0) {
        buffer = (char*) calloc(bytes + 1, sizeof(char));
        if (buffer == NULL) return false;

        if (fread(buffer, sizeof(char), bytes, arquivo) != bytes) {
            free(buffer);
            return false;
        }
    } else {
        buffer = (char*) calloc(1, sizeof(char));
        if (buffer == NULL) return false;
    }

    if (texto != NULL) {
        *texto = buffer;
    } else {
        free(buffer);
    }

    return true;
}

typedef struct {
    FILE *arquivo;
    bool erro;
} IOContextAVL;

// callback chamado para cada NO pela avl_percorrer_em_ordem
static void salvar_no_callback(NO *no, void *ctx_void) {
    IOContextAVL *ctx = (IOContextAVL*) ctx_void;
    if (ctx->erro) return;

    FILE *arquivo = ctx->arquivo;

    // ID
    unsigned int id = avl_obter_ID_no(no);
    if (fwrite(&id, sizeof(unsigned int), 1, arquivo) != 1) {
        ctx->erro = true;
        return;
    }

    // nome
    char *nome = obter_copia_nome_paciente(no);
    if (!salvar_string(arquivo, nome)) {
        destruir_copia_nome_paciente(&nome);
        ctx->erro = true;
        return;
    }
    destruir_copia_nome_paciente(&nome);

    // histórico
    HISTORICO *h = avl_obter_historico_no(no);
    int total_procs = 0;
    if (h != NULL) {
        total_procs = historico_tamanho(h);
        if (total_procs < 0) total_procs = 0;
        if (total_procs > MAX_PROCEDIMENTOS) total_procs = MAX_PROCEDIMENTOS;
    }

    if (fwrite(&total_procs, sizeof(int), 1, arquivo) != 1) {
        ctx->erro = true;
        return;
    }

    for (int i = 0; i < total_procs; i++) {
        PROCEDIMENTO *p = historico_obter_procedimento_por_posicao(h, i);
        const char *desc = procedimento_obter_descricao(p);
        if (!salvar_string(arquivo, desc)) {
            ctx->erro = true;
            return;
        }
    }
}

// SAVE: 
//  Escreve - qtd pacientes, pacientes, Heap, contagor_global
bool SAVE(AVL *dados_AVL, HEAP_DINAMICA *heap, int contador_global) {
    FILE* arquivo = fopen("hospital_data.bin", "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para salvar os dados");
        return false;
    }

    // número de pacientes da AVL
    int n_pacientes = 0;
    if (dados_AVL != NULL) {
        n_pacientes = avl_contar_nos(dados_AVL);
        if (n_pacientes < 0) n_pacientes = 0;
    }

    if (fwrite(&n_pacientes, sizeof(int), 1, arquivo) != 1) {
        fclose(arquivo);
        return false;
    }

    // salvar cada paciente (ID, nome, histórico)
    if (dados_AVL != NULL && n_pacientes > 0) {
        IOContextAVL ctx = { .arquivo = arquivo, .erro = false };
        avl_percorrer_em_ordem(dados_AVL, salvar_no_callback, &ctx);
        if (ctx.erro) {
            fclose(arquivo);
            return false;
        }
    }

    // salvar contador_global (para ordem de chegada futura)
    if (fwrite(&contador_global, sizeof(int), 1, arquivo) != 1) {
        fclose(arquivo);
        return false;
    }

    // salvar heap (fila de prioridade)
    int total_heap = 0;
    if (heap != NULL) {
        total_heap = heap_tamanho(heap);
        if (total_heap < 0) total_heap = 0;
    }

    if (fwrite(&total_heap, sizeof(int), 1, arquivo) != 1) {
        fclose(arquivo);
        return false;
    }

    if (heap != NULL && total_heap > 0) {
        ITEM **buffer = (ITEM**) malloc(total_heap * sizeof(ITEM*));
        if (buffer == NULL) {
            fclose(arquivo);
            return false;
        }

        // desenfileira tudo, grava e guarda num buffer
        for (int i = 0; i < total_heap; i++) {
            ITEM *it = heap_desenfileirar(heap);
            buffer[i] = it;

            int id = heap_item_get_id(it);
            int prioridade = heap_item_get_prioridade(it);
            int ordem = heap_item_get_ordem(it);

            if (fwrite(&id, sizeof(int), 1, arquivo) != 1 ||
                fwrite(&prioridade, sizeof(int), 1, arquivo) != 1 ||
                fwrite(&ordem, sizeof(int), 1, arquivo) != 1) {
                free(buffer);
                fclose(arquivo);
                return false;
            }
        }

        // recoloca os itens na heap, mantendo o estado
        for (int i = 0; i < total_heap; i++) {
            heap_enfileirar(heap, buffer[i]);
        }

        free(buffer);
    }

    fclose(arquivo);
    return true;
}

// LOAD:
//  Leitura - Reconstrói AVL, Heap, contador_global
bool LOAD(AVL **dados_AVL, HEAP_DINAMICA **heap, int *contador_global) {
    if (dados_AVL == NULL || heap == NULL || contador_global == NULL) {
        return false;
    }

    // destruir estruturas antigas, se existirem
    if (*dados_AVL != NULL) {
        avl_apagar(dados_AVL);
        *dados_AVL = NULL;
    }
    if (*heap != NULL) {
        heap_apagar(heap);
        *heap = NULL;
    }

    FILE* arquivo = fopen("hospital_data.bin", "rb");
    if (arquivo == NULL) {
        // arquivo não existe: começa vazio
        printf("Arquivo de dados nao encontrado. Iniciando com sistema vazio.\n");
        *dados_AVL = avl_criar();
        *heap = heap_criar();
        *contador_global = 0;
        return (*dados_AVL != NULL && *heap != NULL);
    }

    AVL *nova_arvore = avl_criar();
    if (nova_arvore == NULL) {
        fclose(arquivo);
        return false;
    }

    int n_pacientes = 0;
    if (fread(&n_pacientes, sizeof(int), 1, arquivo) != 1 || n_pacientes < 0) {
        avl_apagar(&nova_arvore);
        fclose(arquivo);
        return false;
    }

    // carregar pacientes na AVL
    for (int i = 0; i < n_pacientes; i++) {
        unsigned int id = 0;
        if (fread(&id, sizeof(unsigned int), 1, arquivo) != 1) {
            avl_apagar(&nova_arvore);
            fclose(arquivo);
            return false;
        }

        char *nome_lido = NULL;
        if (!carregar_string(arquivo, &nome_lido)) {
            avl_apagar(&nova_arvore);
            fclose(arquivo);
            return false;
        }

        if (!avl_inserir_no(nova_arvore, (nome_lido != NULL) ? nome_lido : "", id)) {
            free(nome_lido);
            avl_apagar(&nova_arvore);
            fclose(arquivo);
            return false;
        }
        free(nome_lido);

        NO *paciente = avl_acha_ID(nova_arvore, id);
        if (paciente == NULL) {
            avl_apagar(&nova_arvore);
            fclose(arquivo);
            return false;
        }

        int total_procs = 0;
        if (fread(&total_procs, sizeof(int), 1, arquivo) != 1 ||
            total_procs < 0 || total_procs > MAX_PROCEDIMENTOS) {
            avl_apagar(&nova_arvore);
            fclose(arquivo);
            return false;
        }

        for (int j = 0; j < total_procs; j++) {
            char *desc_lida = NULL;
            if (!carregar_string(arquivo, &desc_lida)) {
                avl_apagar(&nova_arvore);
                fclose(arquivo);
                return false;
            }

            if (!avl_registrar_procedimento(paciente,
                    (desc_lida != NULL) ? desc_lida : "")) {
                free(desc_lida);
                avl_apagar(&nova_arvore);
                fclose(arquivo);
                return false;
            }
            free(desc_lida);
        }

        // por enquanto, marca fora da fila... Heap vai ajustar depois
        avl_set_esta_na_fila(paciente, false);
    }

    // carregar contador_global
    int contador_lido = 0;
    if (fread(&contador_lido, sizeof(int), 1, arquivo) != 1) {
        avl_apagar(&nova_arvore);
        fclose(arquivo);
        return false;
    }

    // carregar heap
    int total_heap = 0;
    if (fread(&total_heap, sizeof(int), 1, arquivo) != 1 || total_heap < 0) {
        avl_apagar(&nova_arvore);
        fclose(arquivo);
        return false;
    }

    HEAP_DINAMICA *nova_heap = heap_criar();
    if (nova_heap == NULL) {
        avl_apagar(&nova_arvore);
        fclose(arquivo);
        return false;
    }

    int maior_ordem = -1;

    for (int i = 0; i < total_heap; i++) {
        int id = 0, prioridade = 0, ordem = 0;

        if (fread(&id, sizeof(int), 1, arquivo) != 1 ||
            fread(&prioridade, sizeof(int), 1, arquivo) != 1 ||
            fread(&ordem, sizeof(int), 1, arquivo) != 1) {
            avl_apagar(&nova_arvore);
            heap_apagar(&nova_heap);
            fclose(arquivo);
            return false;
        }

        ITEM *it = item_criar(id, prioridade, ordem);
        if (it == NULL || !heap_enfileirar(nova_heap, it)) {
            avl_apagar(&nova_arvore);
            heap_apagar(&nova_heap);
            fclose(arquivo);
            return false;
        }

        // marca na AVL que esse paciente está na fila
        NO *paciente = avl_acha_ID(nova_arvore, (unsigned int)id);
        if (paciente != NULL) {
            avl_set_esta_na_fila(paciente, true);
        }

        if (ordem > maior_ordem) maior_ordem = ordem;
    }

    fclose(arquivo);

    *dados_AVL = nova_arvore;
    *heap = nova_heap;

    // garante continuidade da ordem de chegada
    if (maior_ordem >= contador_lido)
        *contador_global = maior_ordem + 1;
    else
        *contador_global = contador_lido;

    return true;
}
