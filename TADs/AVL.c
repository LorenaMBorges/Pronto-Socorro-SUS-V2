// implementacao do TAD arvore binaria AVL + paciente
#include "AVL.h"

// struct nó, em que cada nó representa um paciente no banco de dados.
struct no {
    // informações do paciente
    char nome[TAM_NOME];
    unsigned int ID;
    bool esta_na_fila;

    // informações gerais de nós
    NO *fesq;
    NO *fdir;
    int altura;
};

// estrutura da árvore AVL.
struct avl {
    NO *raiz;
};


// Cria AVL nova e devolve um ponteiro para ela.
AVL *avl_criar () {
    AVL *arvore = (AVL*) malloc(sizeof(AVL));
    if (arvore != NULL) {
        arvore->raiz = NULL;
    }
    
    return arvore;
}

// Apaga todos os nós da árvore recursivamente (pós-ordem).
void avl_apagar_rec (NO *raiz) {
    if (raiz != NULL) {
        avl_apagar_rec(raiz->fesq);
        avl_apagar_rec(raiz->fdir);
        free(raiz);
    }
}

// Desaloca e desatribui *arvore, e chama avl_apagar_rec para apagar os nós recursivamente.
void avl_apagar (AVL **arvore) {
    if (arvore != NULL && (*arvore) != NULL) {
        avl_apagar_rec((*arvore)->raiz);
        free(*arvore);
        *arvore = NULL;
    }
}

// Devolve o campo altura de um nó, caso o nó não seja nulo. Nesse caso, devolve -1.
int avl_altura_no (NO* no) {
    if (no == NULL) {
        return -1;
    } else {
        return no->altura;
    }
}

// Rotaciona subárvore de raiz a para a direita (rotação simples), além de recalcular as alturas.
NO *rodar_direita (NO *a) {
    NO *b = a->fesq;
    a->fesq = b->fdir;
    b->fdir = a;

    a->altura = max(avl_altura_no(a->fesq), avl_altura_no(a->fdir)) + 1;
    b->altura = max(avl_altura_no(b->fesq), avl_altura_no(b->fdir)) + 1;

    return b;
}

// Rotaciona subárvore de raiz a para a esquerda (rotação simples), além de recalcular as alturas.
NO *rodar_esquerda (NO *a) {
    NO *b = a->fdir;
    a->fdir = b->fesq;
    b->fesq = a;

    a->altura = max(avl_altura_no(a->fesq), avl_altura_no(a->fdir)) + 1;
    b->altura = max(avl_altura_no(b->fesq), avl_altura_no(b->fdir)) + 1;

    return b;
}

// Rotaciona subárvore de raiz a->esq para a esquerda e de raiz a para a direita (rotação dupla esquerda-direita).
NO *rodar_esquerda_direita (NO *a) {
    a->fesq = rodar_esquerda(a->fesq);
    return rodar_direita(a);
}

// Rotaciona subárvore de raiz a->esq para a direita e de raiz a para a esquerda (rotação dupla direita-esquerda).
NO *rodar_direita_esquerda (NO *a) {
    a->fdir = rodar_direita(a->fdir);
    return rodar_esquerda(a);
}

// Encontra o nó com o maior ID na subárvore esquerda.
NO *avl_encontrar_max(NO *subesquerda) {
    NO *maior = subesquerda;

    while (maior != NULL && maior->fdir != NULL) {
        maior = maior->fdir;
    }

    return maior;
}

// Cria nó novo e devolve um ponteiro para esse nó. Só deve ser chamada quando acha-se a posição de inserção.
NO *avl_cria_no (char *NOME, unsigned int ID) {
    NO *novo_no = (NO*) malloc(sizeof(NO));

    if (novo_no != NULL) {
        novo_no->altura = 0;
        novo_no->fdir = NULL;
        novo_no->fesq = NULL;

        novo_no->ID = ID;
        strcpy(novo_no->nome, NOME);
    }

    return novo_no;
}

// De fato insere o nó, procurando recursivamente o local de inserção. Rebalanceia a AVL na volta da recursão.
NO *avl_inserir_rec (NO *raiz, char *NOME, unsigned int ID) {
    if (raiz == NULL)
        raiz = avl_cria_no(NOME, ID);
    else if (ID < raiz->ID)
        raiz->fesq = avl_inserir_rec(raiz->fesq, NOME, ID);
    else if (ID > raiz->ID)
        raiz->fdir = avl_inserir_rec(raiz->fdir, NOME, ID);

    if (raiz!=NULL) {
        raiz->altura = max(avl_altura_no(raiz->fesq), avl_altura_no(raiz->fdir)) + 1;
        int FB = avl_altura_no(raiz->fesq)-avl_altura_no(raiz->fdir);
        
        if (FB == -2) {
            if ((avl_altura_no(raiz->fdir->fesq)-avl_altura_no(raiz->fdir->fdir)) <= 0)
                // se o filho direito também é negativo, ou é zero, rotação simples:
                raiz = rodar_esquerda(raiz);
            else
                // se tem sinais diferentes, precisa de rotação dupla:
                raiz = rodar_direita_esquerda(raiz);
        }
        if (FB == 2) {
            if ((avl_altura_no(raiz->fesq->fesq)-avl_altura_no(raiz->fesq->fdir)) >= 0)
                // se o filho esquerdo também é positivo, ou é zero, rotação simples:
                raiz = rodar_direita(raiz);
            else
                // se tem sinais diferentes, precisa de rotação dupla:
                raiz = rodar_esquerda_direita(raiz);
        }
    }

    return raiz;
}

// Chama recursiva que procura e insere no local correto. Devolve se a raiz é nula ou não.
bool avl_inserir_no (AVL *arvore, char *NOME, unsigned int ID) {
    return ((arvore->raiz = avl_inserir_rec(arvore->raiz, NOME, ID)) != NULL);
}

// Faz a remoção de fato, corrigindo na volta da recursão qualquer desbalanceamento que tenha sido causado.
NO *avl_remover_rec (NO *raiz, unsigned int ID) {
    NO *aux;

    if (raiz == NULL)
        return NULL;
    else if (ID == raiz->ID) {
        // caso tenha 0 ou 1 filho
        if (raiz->fesq == NULL || raiz->fdir == NULL) {
            aux = raiz;
            if (raiz->fesq == NULL)
                raiz = raiz->fdir;
            else
                raiz = raiz->fesq;

            free(aux);
            aux = NULL;
        }
        // caso tenha 2 filhos
        else {
            NO *nova_raiz = avl_encontrar_max(raiz->fesq);

            raiz->ID = nova_raiz->ID;
            strcpy(raiz->nome, nova_raiz->nome);

            raiz->fesq = avl_remover_rec(raiz->fesq, nova_raiz->ID);
        }
            
    }
    else if (ID < raiz->ID)
        raiz->fesq = avl_remover_rec(raiz->fesq, ID);
    else if (ID > raiz->ID)
        raiz->fdir = avl_remover_rec(raiz->fdir, ID);

    if (raiz != NULL) {
        raiz->altura = max(avl_altura_no(raiz->fesq), avl_altura_no(raiz->fdir)) + 1;
        
        int FB = avl_altura_no(raiz->fesq)-avl_altura_no(raiz->fdir);
        if (FB == -2) {
            if ((avl_altura_no(raiz->fdir->fesq)-avl_altura_no(raiz->fdir->fdir)) <= 0)
                // se o filho direito também é negativo, ou é zero, rotação simples:
                raiz = rodar_esquerda(raiz);
            else
                // se tem sinais diferentes, precisa de rotação dupla:
                raiz = rodar_direita_esquerda(raiz);
        }
        if (FB == 2) {
            if ((avl_altura_no(raiz->fesq->fesq)-avl_altura_no(raiz->fesq->fdir)) >= 0)
                // se o filho esquerdo também é positivo, ou é zero, rotação simples:
                raiz = rodar_direita(raiz);
            else
                // se tem sinais diferentes, precisa de rotação dupla:
                raiz = rodar_esquerda_direita(raiz);
        }
    }

    return raiz;
}

// Chama a função recursiva de remoção e devolve se a raiz é ou não nula.
bool avl_remover_no (AVL *T, unsigned int ID) {
    return((T->raiz = avl_remover_rec(T->raiz, ID)) != NULL);
}

// Imprime todos os nós da árvore em ordem pelos IDs.
void avl_imprimir_2 (NO* raiz) {
    if (raiz != NULL) {
        avl_imprimir_2(raiz->fesq);
        printf("\nPaciente de ID %u:\n", raiz->ID);
        printf("Nome: %s\n", raiz->nome);
        avl_imprimir_2(raiz->fdir);
    }
}

// Adapta a enterada de AVL para raiz.
void avl_imprimir(AVL* arvore) {
    avl_imprimir_2(arvore->raiz);
}

// Procura nó de mesmo ID recursivamente. Caso encontre, devolve um ponteiro para esse nó, caso contrário, devolve NULL.
NO* avl_acha_ID_rec (NO *raiz, unsigned int ID) {
    if (raiz != NULL) {
        if (raiz->ID == ID) {
            return raiz;
        }
        else
            if (raiz->ID > ID)
                return avl_acha_ID_rec(raiz->fesq, ID);
            if (raiz->ID < ID)
                return avl_acha_ID_rec(raiz->fdir, ID);
    }
    else
        return NULL;
}

// Devolve se um ID está disponível para uso. Caso ele já exista, não está, caso contrário, pode ser adotado, estão está disponível.
bool avl_ID_disponivel (NO *raiz, unsigned int ID) {
    return ((avl_acha_ID_rec(raiz, ID)) == NULL);
}

// Chama função de busca.
// Pode ser utilizado para buscar um paciente no banco de dados.
NO* avl_acha_ID (AVL *arvore, unsigned int ID) {
    if (arvore != NULL)
        return(avl_acha_ID_rec(arvore->raiz, ID));
    
    return NULL;
}

// Cria e devolve cópia do nome do paciente correspodente a aquele nó.
char *obter_copia_nome_paciente (NO* no) {
    if (no != NULL) {
        char* copia = (char*) malloc(sizeof(char)*TAM_NOME);
        strcpy(copia, no->nome);

        return copia;
    }

    return NULL;
}

// Destroi cópia do nome do paciente.
void destruir_copia_nome_paciente (char **copia) {
    if (copia != NULL && (*copia) != NULL) {
        free(*copia);
        *copia = NULL;
    }
}

void avl_set_esta_na_fila(NO *paciente, bool estado){
    if(paciente != NULL){
        paciente->esta_na_fila = estado;
    }
}

bool avl_obter_esta_na_fila_no(NO* no){
    if(no != NULL){
        return no->esta_na_fila;
    }
    return false;
}