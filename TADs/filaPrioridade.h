// interface do TAD fila de prioridade
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "AVL.h"

typedef struct item_heap ITEM;
typedef struct heap_dinamica HEAP_DINAMICA;

HEAP_DINAMICA *heap_criar();
ITEM* item_criar(unsigned int id, unsigned int prioridade, unsigned int ordem);

bool heap_cheia(HEAP_DINAMICA *heap);
bool heap_vazia(HEAP_DINAMICA *heap);
void heap_aumentar(HEAP_DINAMICA *heap);

void heap_swap(HEAP_DINAMICA *heap, int i, int j);
void heap_fix_up(HEAP_DINAMICA *heap);
void heap_fix_down(HEAP_DINAMICA *heap);

bool heap_enfileirar(HEAP_DINAMICA *heap, ITEM *item);
ITEM* heap_desenfileirar(HEAP_DINAMICA *heap);

void heap_printar(HEAP_DINAMICA *heap, AVL *dados_AVL);

int heap_item_get_id(ITEM *item);
int heap_item_get_prioridade(ITEM *item);
int heap_item_get_ordem(ITEM *item);

// Usados no IO
int heap_tamanho(HEAP_DINAMICA *heap);
void heap_apagar(HEAP_DINAMICA **heap);

