// interface do TAD fila de prioridade
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "AVL.h"

typedef struct item_heap ITEM;
typedef struct heap_dinamica HEAP_DINAMICA;

HEAP_DINAMICA *heap_criar(); //Heap mínima: nó >= pai(nó)
ITEM* item_criar(int id, int prioridade, int ordem);

bool heap_cheia(HEAP_DINAMICA *heap);
bool heap_vazia(HEAP_DINAMICA *heap);
void heap_aumentar(HEAP_DINAMICA *heap); //Com a heap dinamica, quando houver problema de capacidade, é só aumentar

void heap_swap(HEAP_DINAMICA *heap, int i, int j);
void heap_fix_up(HEAP_DINAMICA *heap); //A chave para manter a ordem será ITEM->prioridade
void heap_fix_down(HEAP_DINAMICA *heap);

bool heap_enfileirar(HEAP_DINAMICA *heap, ITEM *item); //Adicionar paciente na fila de atendimento
ITEM* heap_desenfileirar(HEAP_DINAMICA *heap); //Paciente foi atendido

void heap_printar(HEAP_DINAMICA *heap); //Listar pacientes na fila de acordo com a prioridade
