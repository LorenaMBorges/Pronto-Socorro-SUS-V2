// codigo principal do Pronto Socorro SUS V2

#include <stdio.h>
#include <stdlib.h>
#include "./TADs/AVL.h"
#include "./TADs/filaPrioridade.h"

int contador_global = 0;

int main(){
    int escolha_menu;
    HEAP_DINAMICA *heap = heap_criar();
    // precisa carregar os dados. Acho que seria interessante a fila vir dos dados também, e não criar uma fila nova.
    // chamei o ponteiro para a AVL de dados_AVL:
    // AVL *dados_AVL;

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
            { // precisa adicionar paciente na AVL
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
            {
                unsigned int ID;
                printf("Digite o ID: ");
                scanf("%u", &ID);

                if (/*função de verificação para saber se a pessoa está na fila de espera ou não*/) {
                    if(avl_remover_no (dados_AVL, ID)) {
                        printf("\nRemocao bem-sucedida.\n");
                    }
                    else 
                        printf("\nRemocao falhou. Tente novamente.");
                }
                else
                    printf("\nEsse paciente nao pode ser removido do sistema pois esta na fila de espera.\n");

                break;
            }
        case 3: vl_imprimir(dados_AVL);
            break;
        case 4:
            {
                unsigned int ID;
                printf("Digite o ID: ");
                scanf("%u", &ID);

                NO* paciente = avl_acha_ID(dados_AVL, ID);
                if (paciente != NULL) {
                    printf("\nPaciente encontrado:\nNome: ");
                    char nome[TAM_NOME] = obter_copia_nome_paciente(paciente);
                    printf("%s\n", nome);
                    destruir_copia_nome_paciente(nome);
                }
                else
                    printf("\nPaciente com ID %d nao encontrado.\n", ID);

                break;
            }
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