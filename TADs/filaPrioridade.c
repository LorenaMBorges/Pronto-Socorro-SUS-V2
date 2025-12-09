// implementacao do TAD fila de prioridade (heap dinâmico com arranjo)
#include "filaPrioridade.h"

struct item_heap{ //id do paciente, categoria de prioridade e decisão de desempate
    int id;
    int prioridade;
    int ordem_chegada;
};  

struct heap_dinamica{ //arranjo para implementação, seu tamanho real e capacidade de alocação
    ITEM **arranjo;
    int tamanho;
    int capacidade;
};

HEAP_DINAMICA *heap_criar(){ //Heap mínima: nó >= pai(nó)
    HEAP_DINAMICA *heap = (HEAP_DINAMICA*) malloc(sizeof(HEAP_DINAMICA));
    if(heap != NULL){
        heap->capacidade = 32; //tamanho inicial fixa
        heap->tamanho = 0;
        heap->arranjo = malloc(heap->capacidade * sizeof(ITEM*));
    }
    return heap;
}

ITEM* item_criar(unsigned int id, unsigned int prioridade, unsigned int ordem){
    ITEM *item = malloc(sizeof(ITEM));
    if(item != NULL){
        item->id = id;
        item->prioridade = prioridade;
        item->ordem_chegada = ordem;
    }
    return item;
}

bool heap_cheia(HEAP_DINAMICA *heap){
    if(heap->capacidade == heap->tamanho){
        return true;
    }
    return false;
}

bool heap_vazia(HEAP_DINAMICA *heap){
    if(heap->tamanho == 0){
        return true;
    }
    return false;
}

void heap_aumentar(HEAP_DINAMICA *heap){ //Com a heap dinamica, quando houver problema de capacidade, é só aumentá-la
    int nova_capacidade = heap->capacidade * 2;

    ITEM **novo = realloc(
        heap->arranjo,
        nova_capacidade * sizeof(ITEM*)
    );

    if(novo == NULL){
        return;
    }

    heap->arranjo = novo;
    heap->capacidade = nova_capacidade;

    return;
}

void heap_swap(HEAP_DINAMICA *heap, int i, int j){
    ITEM *tmp = heap->arranjo[i];
    heap->arranjo[i] = heap->arranjo[j];
    heap->arranjo[j] = tmp;
}

void heap_fix_up(HEAP_DINAMICA *heap){ //A chave para manter a ordem será ITEM->prioridade (heap mínima)
    int i = heap->tamanho - 1;

    while(i > 0){
        int pai = (i - 1) / 2;

        ITEM *filho = heap->arranjo[i];
        ITEM *no_pai = heap->arranjo[pai];

        if( //regra de prioridade para heap mínima
            filho->prioridade < no_pai->prioridade || 
            (filho->prioridade == no_pai->prioridade &&
            filho->ordem_chegada < no_pai->ordem_chegada) //ordem de chegada como desempate
        ){
            //troca
            heap_swap(heap, i, pai);
            i = pai;
        }else{
            break; //ordem satisfeita
        }
    }
}

void heap_fix_down(HEAP_DINAMICA *heap){
    int i = 0;

    while(true){
        int esq = 2*i + 1;
        int dir = 2*i + 2;
        int menor = i;

        // compara com filho esquerdo
        if(esq < heap->tamanho){
            ITEM *f = heap->arranjo[esq];
            ITEM *p = heap->arranjo[menor];

            if(
                f->prioridade < p->prioridade ||
               (f->prioridade == p->prioridade &&
                f->ordem_chegada < p->ordem_chegada)
            ){
                menor = esq;
            }
        }

        // compara com filho direito
        if(dir < heap->tamanho){
            ITEM *f = heap->arranjo[dir];
            ITEM *p = heap->arranjo[menor];

            if(
                f->prioridade < p->prioridade ||
               (f->prioridade == p->prioridade &&
                f->ordem_chegada < p->ordem_chegada)
            ){
                menor = dir;
            }
        }

        if(menor == i){
            break;   // ordem satisfeita
        }

        heap_swap(heap, i, menor);
        i = menor;
    }
}

bool heap_enfileirar(HEAP_DINAMICA *heap, ITEM *item){ //Adicionar paciente na fila de atendimento
    if(heap_cheia(heap)){
        heap_aumentar(heap);
    }
    heap->arranjo[heap->tamanho] = item;
    heap->tamanho++;

    heap_fix_up(heap);
    return true;
}

ITEM* heap_desenfileirar(HEAP_DINAMICA *heap){ //Paciente foi atendido
    if(heap_vazia(heap)){
        return NULL;
    }
    ITEM *raiz = heap->arranjo[0]; //guardando para retorno
    heap->arranjo[0] = heap->arranjo[heap->tamanho - 1];
    heap->arranjo[heap->tamanho - 1] = NULL;
    heap->tamanho--;

    heap_fix_down(heap);

    return raiz;
}

void heap_printar(HEAP_DINAMICA *heap, AVL *dados_AVL){ // cria cópia da heap para printar a remoção na ordem
    if(heap_vazia(heap)){
        printf("A fila atual se encontra vazia.\n");
        return;
    }

    printf("\nFILA DE ESPERA ATUAL:\n");
    printf("[prioridade | paciente]\n");

    HEAP_DINAMICA copia;
    copia.capacidade = heap->capacidade;
    copia.tamanho = heap->tamanho;
    copia.arranjo = malloc(copia.capacidade * sizeof(ITEM*));

    // copia os ponteiros
    for(int i = 0; i < heap->tamanho; i++){
        copia.arranjo[i] = heap->arranjo[i];
    }

    // remove na ordem correta
    while(!heap_vazia(&copia)){
        ITEM *p = heap_desenfileirar(&copia);

        NO *paciente = avl_acha_ID(dados_AVL, p->id);
        char *nome = paciente ? obter_copia_nome_paciente(paciente) : "Desconhecido";

        switch(p->prioridade){
            case 1: printf("Emergencia - %s\n", nome);        break;
            case 2: printf("Muito Urgente - %s\n", nome);     break;
            case 3: printf("Urgente - %s\n", nome);           break;
            case 4: printf("Pouco Urgente - %s\n", nome);     break;
            case 5: printf("Nao Urgente - %s\n", nome);       break;
            default:                                          break;
        }

        if(paciente) destruir_copia_nome_paciente(&nome);
    }

    free(copia.arranjo);
}

int heap_item_get_id(ITEM *item){
    return item->id;
}

int heap_item_get_prioridade(ITEM *item){
    return item->prioridade;
}

int heap_item_get_ordem(ITEM *item){
    return item->ordem_chegada;
}

int heap_tamanho(HEAP_DINAMICA *heap) {
    if (heap == NULL) return 0;
    return heap->tamanho;
}

void heap_apagar(HEAP_DINAMICA **heap_ref) {
    if (heap_ref == NULL || *heap_ref == NULL) return;

    HEAP_DINAMICA *heap = *heap_ref;

    // libera cada ITEM
    for (int i = 0; i < heap->tamanho; i++) {
        if (heap->arranjo[i] != NULL) {
            free(heap->arranjo[i]);
        }
    }

    // libera o vetor de ponteiros e a própria heap
    free(heap->arranjo);
    free(heap);

    *heap_ref = NULL;
}

