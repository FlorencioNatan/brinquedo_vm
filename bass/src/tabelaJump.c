#include <string.h>
#include <stdlib.h>
#include "tabelaJump.h"
#include "instrucoes.h"

#define ESTADO_PROCESSAMANETO_INVALIDO 0
#define ESTADO_PROCESSAMANETO_CODE 1
#define ESTADO_PROCESSAMANETO_DATA 2


jump* montar_tabela_jumps(char *assembly, int totalJumps) {
    jump *tabelaJumps = (jump*)malloc(totalJumps * sizeof(jump));
    int posicaoPrograma = 0;
    int indiceJump = 0;
    int estadoProcessamento = 0;
    char *token;
    char *assemblyCopy = malloc(strlen(assembly));
    strcpy(assemblyCopy, assembly);

    token = strtok (assemblyCopy," \t\n,");
    do {
        if (token == NULL) {
            continue;
        }

        if (strcmp(token, ".code") == 0) {
            estadoProcessamento = ESTADO_PROCESSAMANETO_CODE;
            token = strtok(NULL," \t\n,");
            continue;
        }

        if (strcmp(token, ".data") == 0) {
            estadoProcessamento = ESTADO_PROCESSAMANETO_DATA;
            token = strtok(NULL," \t\n,");
            continue;
        }

        if (estadoProcessamento == ESTADO_PROCESSAMANETO_DATA) {
            token = strtok(NULL," \t\n,");
            continue;
        }

        if (token[strlen(token)-1] == ':') {
            tabelaJumps[indiceJump].label = malloc(strlen(token)-1);
            strncpy(tabelaJumps[indiceJump].label, token, strlen(token)-1);
            tabelaJumps[indiceJump].posicao = posicaoPrograma;
            indiceJump++;
            token = strtok (NULL," \t\n,");
            continue;
        }

        registroInstrucao instrucao = lookup_instrucao(token);
        posicaoPrograma += instrucao.tamanho;
        token = strtok (NULL," \t\n,");
    } while (token != NULL);
    free(assemblyCopy);
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
