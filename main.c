// codigo principal do Pronto Socorro SUS V2

#include <stdio.h>
#include <stdlib.h>
#include "./TADs/AVL.h"
#include "./TADs/filaPrioridade.h"

int contador_global = 0;

void registrar_paciente(AVL *dados_AVL, HEAP_DINAMICA *heap);
void remover_paciente(AVL *dados_AVL);
void listar_pacientes(AVL *dados_AVL);
void buscar_paciente_por_id(AVL *dados_AVL);
void mostrar_fila_de_espera(HEAP_DINAMICA *heap);
void dar_alta_ao_paciente(AVL *dados_AVL, HEAP_DINAMICA *heap);
void sair();

int main(){
    int escolha_menu;
    HEAP_DINAMICA *heap = heap_criar();
    // precisa carregar os dados. Acho que seria interessante a fila vir dos dados também, e não criar uma fila nova.
    // chamei o ponteiro para a AVL de dados_AVL:
    AVL *dados_AVL;

    printf("\nBem vindo Sistema do SUS!\n");

    while(true){
        printf("\nPor favor escolha entre as seguintes opcoes:\n");
        printf("[1] Registrar paciente\n");
        printf("[2] Remover paciente\n");
        printf("[3] Listar pacientes\n");
        printf("[4] Buscar paciente por ID\n");
        printf("[5] Mostrar fila de espera\n");
        printf("[6] Dar alta ao paciente\n");
        printf("[7] Sair\n\n");

        printf("Por favor, escolha uma opcao: ");
        scanf("%d", &escolha_menu);

        switch (escolha_menu) {
        case 1: registrar_paciente(dados_AVL, heap);    break;
        case 2: remover_paciente(dados_AVL);            break;
        case 3: avl_imprimir(dados_AVL);                break;
        case 4: buscar_paciente_por_id(dados_AVL);      break;
        case 5: heap_printar(heap);                     break;
        case 6: dar_alta_ao_paciente(dados_AVL, heap);  break;
        case 7: sair();                                 break;
        default: 
            break;
        }
    }
}

void registrar_paciente(AVL *dados_AVL, HEAP_DINAMICA *heap){
    unsigned int id, prioridade;

    printf("\nOpção 1 Escolhida: Vamos registrar um paciente\n");
    
    printf("Digite o ID: ");
    scanf("%u", &id);

    printf("Digite a prioridade (1-5): ");
    scanf("%u", &prioridade);
    
    // precisa adicionar paciente na AVL
    
    ITEM *item = item_criar(id, prioridade, contador_global++);

    //registrar que o paciente entrou na fila
    heap_enfileirar(heap, item);

    return;
}

void remover_paciente(AVL *dados_AVL){
    unsigned int id;
    
    printf("\nOpção 2 Escolhida: Vamos remover um paciente dos registros\n");

    printf("Digite o ID: ");
    scanf("%u", &id);

    if (/*função de verificação para saber se a pessoa está na fila de espera ou não*/) {
        if(avl_remover_no (dados_AVL, id)) {
            printf("\nRemocao bem-sucedida.\n");
        }
        else 
            printf("\nRemocao falhou. Tente novamente.");
    }
    else
        printf("\nEsse paciente nao pode ser removido do sistema pois esta na fila de espera.\n");

    return;
}

void listar_pacientes(AVL *dados_AVL){
    avl_imprimir(dados_AVL);
    return;
}

void buscar_paciente_por_id(AVL *dados_AVL){
    unsigned int id;

    printf("\nOpção 4 Escolhida: Vamos buscar um paciente pelo ID\n");

    printf("Digite o ID: ");
    scanf("%u", &id);

    NO* paciente = avl_acha_ID(dados_AVL, id);

    if (paciente != NULL) {
        printf("\nPaciente encontrado:\nNome: ");

        char nome[TAM_NOME] = obter_copia_nome_paciente(paciente);
        printf("%s\n", nome);

        destruir_copia_nome_paciente(nome);
    }
    else{
        printf("\nPaciente com ID %d nao encontrado.\n", id);
    }
}

void mostrar_fila_de_espera(HEAP_DINAMICA *heap){
    heap_printar(heap);
    return;
}

void dar_alta_ao_paciente(AVL *dados_AVL, HEAP_DINAMICA *heap){
    if(heap_vazia(heap)){
        printf("\nNao ha pacientes na fila.\n");
        return;
    }
    
    ITEM *item_removido = heap_desenfileirar(heap);
    //registrar que o paciente saiu da fila

    int id = heap_item_get_id(item_removido);
    NO *paciente = avl_acha_ID(dados_AVL, id);

    printf("\nPaciente atendido:\n");
    if(paciente != NULL){
        char *nome = obter_copia_nome_paciente(paciente);

        printf("Nome: %s\n", nome);
        printf("ID: %d\n", id);

        destruir_copia_nome_paciente(&nome);
    }
    else{
        printf("Paciente com id %d nao encontrado no cadastro.\n", id);
    }

    free(item_removido);

    return;
}

void sair(){
    printf("\nSistema fechando...\n");
    return;
}