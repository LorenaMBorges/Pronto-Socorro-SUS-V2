#ifndef HISTORICO_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    typedef struct historico HISTORICO;
    typedef struct procedimento PROCEDIMENTO;

    #define MAX_PROCEDIMENTOS 10
    #define MAX_DESCRICAO 101

    HISTORICO* historico_criar();

    bool inserir_procedimento(HISTORICO* h, PROCEDIMENTO* p);
    PROCEDIMENTO* desfazer_procedimento(HISTORICO* h);
    void historico_consultar(HISTORICO* h); 
    
    bool historico_apagar(HISTORICO** h);
    bool historico_vazio(HISTORICO* h);
    bool historico_cheio(HISTORICO* h);
    int historico_tamanho(HISTORICO* h);
    PROCEDIMENTO* historico_obter_procedimento_por_posicao(HISTORICO* h, int pos);
    PROCEDIMENTO* procedimento_criar(const char descricao[]);
    const char* procedimento_obter_descricao(PROCEDIMENTO* procedimento);
    bool procedimento_apagar(PROCEDIMENTO** p);
#endif