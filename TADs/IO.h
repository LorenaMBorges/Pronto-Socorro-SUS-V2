#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include "AVL.h"
#include "filaPrioridade.h"

// SAVE: salva AVL + Heap + histórico em "hospital_data.bin"
bool SAVE(AVL *dados_AVL, HEAP_DINAMICA *heap, int contador_global);

// LOAD: carrega AVL + Heap a partir do arquivo, reconstruindo contador_global
bool LOAD(AVL **dados_AVL, HEAP_DINAMICA **heap, int *contador_global);

#endif
