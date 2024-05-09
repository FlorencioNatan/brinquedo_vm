#include <string.h>
#include <stdlib.h>
#include "tabelaJump.h"
#include "instrucoes.h"

jump* montar_tabela_jumps(FILE *arquivoBass, int totalJumps) {
    char linha[100];
    jump *tabelaJumps = (jump*)malloc(totalJumps * sizeof(jump));
    int posicaoPrograma = 0;
    int indiceJump = 0;

    while(fgets(linha, 100, arquivoBass)) {
        char *token;
        token = strtok (linha," \t\n,.");

        if (token == NULL) {
            continue;
        }

        if (token[strlen(token)-1] == ':') {
            tabelaJumps[indiceJump].label = malloc(strlen(token)-1);
            strncpy(tabelaJumps[indiceJump].label, token, strlen(token)-1);
            tabelaJumps[indiceJump].posicao = posicaoPrograma;
            indiceJump++;
            continue;
        }

        registroInstrucao instrucao = lookup_instrucao(token);
        posicaoPrograma += instrucao.tamanho;
    }

    return tabelaJumps;
}

jump* lookup_jump(jump *tabelaJumps, int totalJumps, char* label) {
    for (int i = 0; i < totalJumps; i++) {
        char *labelJump = tabelaJumps[i].label;
        if (strncmp(labelJump, label, strlen(labelJump)) == 0) {
            return &tabelaJumps[i];
        }
    }
    return NULL;
}