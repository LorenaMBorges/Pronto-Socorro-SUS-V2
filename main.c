// codigo principal do Pronto Socorro SUS V2

#include <stdio.h>
#include "./TADs/AVL.h"
#include "./TADs/filaPrioridade.h"

int main(){
    int escolha_menu;

    printf("\nBem vindo Sistema do SUS!\n");
    printf("Por favor escolha entre as seguintes opcoes:\n");
    printf("[1] Registrar paciente\n");
    printf("[2] Remover paciente\n");
    printf("[3] Listar pacientes\n");
    printf("[4] Buscar paciente por ID\n");
    printf("[5] Mostrar fila de espera\n");
    printf("[6] Dar alta ao paciente\n");
    printf("[7] Sair\n\n");

    printf("Por favor, escolha uma opcao: ");
    scanf("%d", escolha_menu);

    switch (escolha_menu) {
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    default:
        break;
    }
}