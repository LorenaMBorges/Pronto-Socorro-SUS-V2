/* TRABALHO 2 DA DISCIPLINA DE ALGORITMOS E ESTRUTURAS DE DADOS

Alunos:
João Pedro Boaretto, nUSP:  16876293
Lorena Moreira Borges, nUSP: 16883652
Sofia Albuquerque Lima, nUSP: 16900810

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./TADs/AVL.h"
#include "./TADs/filaPrioridade.h"
#include "./TADs/IO.h"

int contador_global = 0;

void registrar_paciente(AVL *dados_AVL, HEAP_DINAMICA *heap);
void remover_paciente(AVL *dados_AVL);
void listar_pacientes(AVL *dados_AVL);
void buscar_paciente_por_id(AVL *dados_AVL);
void mostrar_fila_de_espera(HEAP_DINAMICA *heap);
void dar_alta_ao_paciente(AVL *dados_AVL, HEAP_DINAMICA *heap);
int sair(void);

int main(){
    int escolha_menu; 
    int rodando = 1;
    HEAP_DINAMICA *heap = heap_criar();
    // precisa carregar os dados. Acho que seria interessante a fila vir dos dados também, e não criar uma fila nova.
    // chamei o ponteiro para a AVL de dados_AVL:
    AVL *dados_AVL;

    printf("\nBem vindo Sistema do SUS!\n");

    while(rodando){
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
        case 3: listar_pacientes(dados_AVL);            break;
        case 4: buscar_paciente_por_id(dados_AVL);      break;
        case 5: mostrar_fila_de_espera(heap);           break;
        case 6: dar_alta_ao_paciente(dados_AVL, heap);  break;
        case 7: rodando = sair();                       break;
        default: break;
        }
    }
}

void registrar_paciente(AVL *dados_AVL, HEAP_DINAMICA *heap){
    unsigned int id, prioridade;

    printf("\nOpcao 1 Escolhida: Vamos registrar um paciente...\n");
    
    printf("Digite o ID: ");
    scanf("%u", &id);

    if (avl_ID_disponivel(dados_AVL, id)) {
        printf("\nPaciente ainda não registrado.\n");

        printf("Digite o nome: ");
        char nome[TAM_NOME];
        scanf(" %s", nome);

        if(avl_inserir_no(dados_AVL, nome, id))
            printf("\n\nPaciente inserido no registro.\n");
        else {
            printf("\n\nErro de registro do paciente. Tente novamente.\n");
            return;
        }
    }
    else {
        printf("\nID ja utilizado, paciente ja registrado.\n");
    }

    printf("Digite a prioridade (1-5): ");
    scanf("%u", &prioridade);

    NO *add_fila = avl_acha_ID(dados_AVL, id);

    if(avl_obter_esta_na_fila_no(add_fila)){
        ITEM *item = item_criar(id, prioridade, contador_global++);
        heap_enfileirar(heap, item);
        avl_set_esta_na_fila(avl_acha_ID(dados_AVL, id), true);

        printf("\nPaciente adicionado na fila com sucesso!\n");
    }else{
        printf("\nPaciente ja esta na fila!\n");
    }

    return;
}

void remover_paciente(AVL *dados_AVL){
    unsigned int id;
    
    printf("\nOpcao 2 Escolhida: Vamos remover um paciente dos registros...\n");

    printf("Digite o ID: ");
    scanf("%u", &id);

    bool esta_na_fila = avl_obter_esta_na_fila_no(avl_acha_ID(dados_AVL, id));

    if (esta_na_fila) {
        if(avl_remover_no(dados_AVL, id)) {
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
    char escolha;

    printf("\nOpcao 3 Escolhida: Vamos visualizar os pacientes cadastrados...\n");
    
    printf("Você deseja ver o histórico de procedimentos dos pacientes? [Y/N]");
    scanf(" %c", &escolha);

    if(escolha == 'y' || escolha == 'Y'){
        avl_imprimir_com_historico(dados_AVL);
    }else{
        avl_imprimir(dados_AVL);
    }

    return;
}

void buscar_paciente_por_id(AVL *dados_AVL){
    unsigned int id;

    printf("\nOpcao 4 Escolhida: Vamos buscar um paciente pelo ID...\n");

    printf("Digite o ID: ");
    scanf("%u", &id);

    NO* paciente = avl_acha_ID(dados_AVL, id);

    if (paciente != NULL) {
        printf("\nPaciente encontrado:\nNome: ");

        char *nome = obter_copia_nome_paciente(paciente);
        printf("%s\n", nome);

        destruir_copia_nome_paciente(&nome);
    }
    else{
        printf("\nPaciente com ID %d nao encontrado.\n", id);
    }
}

void mostrar_fila_de_espera(HEAP_DINAMICA *heap){
    printf("\nOpcao 5 Escolhida: Vamos visualizar a fila de espera...\n");
    heap_printar(heap);
    return;
}

void dar_alta_ao_paciente(AVL *dados_AVL, HEAP_DINAMICA *heap){
    printf("\nOpcao 6 Escolhida: Vamos visualizar a fila de espera...\n");

    if(heap_vazia(heap)){
        printf("\nNao ha pacientes na fila.\n");
        return;
    }
    
    ITEM *item_removido = heap_desenfileirar(heap);

    int id = heap_item_get_id(item_removido);
    NO *paciente = avl_acha_ID(dados_AVL, id);

    avl_set_esta_na_fila(paciente, false);

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

int sair(void){
    printf("\nSistema fechando...\n");
    return 0;
}