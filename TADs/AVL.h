// interface do TAD arvore binaria AVL + paciente
#define max(a, b) ((a > b) ? a : b)
#define TAM_NOME 100

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct avl AVL;
typedef struct no NO;

AVL *avl_criar ();
void avl_apagar (AVL **arvore);
bool avl_inserir_no (AVL *arvore, char *NOME, unsigned int ID);
bool avl_remover_no (AVL *T, unsigned int ID);
void avl_imprimir (NO* raiz);
bool avl_ID_disponivel (NO *raiz, unsigned int ID);
NO* avl_acha_ID (NO *raiz, unsigned int ID);
char *obter_copia_nome_paciente (NO* no);
void destruir_copia_nome_paciente (char **copia);