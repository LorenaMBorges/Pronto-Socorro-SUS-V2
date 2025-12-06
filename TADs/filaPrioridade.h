// interface do TAD fila de prioridade
#include <stdio.h>
#include <stdbool.h>
#include "AVL.h"

int heap_enfileirar(); //Adicionar paciente na fila de atendimento
void heap_printar(); //Listar pacientes na fila de acordo com a prioridade
bool heap_desenfileirar(); //Paciente foi atendido