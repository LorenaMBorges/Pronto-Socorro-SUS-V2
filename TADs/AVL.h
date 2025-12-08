// interface do TAD arvore binaria AVL + paciente
#define max(a, b) ((a > b) ? a : b)
#define TAM_NOME 100

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "historico.h"

typedef struct avl AVL;
typedef struct no NO;

AVL *avl_criar ();
void avl_apagar (AVL **arvore);
bool avl_inserir_no (AVL *arvore, char *NOME, unsigned int ID);
bool avl_remover_no (AVL *T, unsigned int ID);
void avl_imprimir (AVL* arvore);
void avl_imprimir_com_historico(AVL* arvore);
bool avl_ID_disponivel (AVL *arvore, unsigned int ID);
NO* avl_acha_ID (AVL *arvore, unsigned int ID);
char *obter_copia_nome_paciente (NO* no);
void destruir_copia_nome_paciente (char **copia);

void avl_set_esta_na_fila(NO *paciente, bool estado);
bool avl_obter_esta_na_fila_no(NO* no);

bool avl_registrar_procedimento(NO *paciente, const char *descricao);
void avl_imprimir_historico(NO *paciente);

// percorre a árvore em-ordem e chama callback para cada nó
void avl_percorrer_em_ordem(AVL *arvore, void (*callback)(NO *no, void *ctx), void *ctx);

// quantidade total de nós na AVL
int avl_contar_nos(AVL *arvore);

// getters simples para IO
unsigned int avl_obter_ID_no(NO *no);
HISTORICO* avl_obter_historico_no(NO *no);