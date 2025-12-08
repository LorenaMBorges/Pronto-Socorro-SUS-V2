// codigo principal do Pronto Socorro SUS V2

#include <stdio.h>
#include <stdlib.h>
#include "./TADs/AVL.h"
#include "./TADs/filaPrioridade.h"

int contador_global = 0;

int main(){
    int escolha_menu;
    HEAP_DINAMICA *heap = heap_criar();

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
        case 1:
            {
                int id, prioridade;
                printf("Digite o ID: ");
                scanf("%d", &id);

                printf("Digite a prioridade (1-5): ");
                scanf("%d", &prioridade);
                
                ITEM *item = item_criar(id, prioridade, contador_global++);

                heap_enfileirar(heap, item);
                break;
            }
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5: heap_printar(heap);
            break;
        case 6:
            break;
        case 7: {
            printf("\nSistema fechando...");
            return 0;
            }
            break;
        default: 
            break;
        }
    }
}