#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "instrucoes.h"

typedef struct jump {
    char* label;
    uint64_t posicao;
} jump;

void totalizar_instrucoes(
    FILE *arquivoBass,
    int *totalJumps,
    int *tamanhoArquivo
) {
    char linha[100];

    while(fgets(linha, 100, arquivoBass)) {
        char *token;
        token = strtok (linha," \t\n,.");

        if (token == NULL) {
            continue;
        }

        if (token[strlen(token)-1] == ':') {
            (*totalJumps)++;
            continue;
        }
        registroInstrucao instrucao = lookup_instrucao(token);
        (*tamanhoArquivo) += instrucao.tamanho;
    };
}

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

void processar_linha_assembly(char linha[100], jump *tabelaJumps, int totalJumps, uint8_t* conteudo, int *indiceConteudo){
    char *token;
    token = strtok(linha," \t\n,.");

    if (token == NULL) {
        return;
    }

    if (token[strlen(token)-1] == ':') {
        return;
    }

    registroInstrucao instrucao = lookup_instrucao(token);
    token = strtok(NULL, " \t\n,.");
    if (token != NULL) {
        if ((token[0] >= '0' && token[0] <= '9') || token[0] == '-' || token[0] == '+') {
            conteudo[(*indiceConteudo)++] = instrucao.codigo;
            int64_t parametro = strtol(token, NULL, 0);
            uint8_t *byteParametro = (uint8_t*)&parametro;
            for (uint64_t i = 0; i < sizeof(uint64_t); i++) {
                conteudo[(*indiceConteudo)++] = byteParametro[i];
            }
            return;
        }

        for (int i = 0; i < totalJumps; i++) {
            char *label = tabelaJumps[i].label;
            if (strncmp(label, token, strlen(label)) == 0) {
                conteudo[(*indiceConteudo)++] = INST_PUSH;
                uint8_t *bytePosicao = (uint8_t*)&tabelaJumps[i].posicao;
                for (uint64_t i = 0; i < sizeof(uint64_t); i++) {
                    conteudo[(*indiceConteudo)++] = bytePosicao[i];
                }
                conteudo[(*indiceConteudo)++] = instrucao.codigo;
                return;
            }
        }
    }

    conteudo[(*indiceConteudo)++] = instrucao.codigo;
}

uint8_t* gerar_conteudo_binario(
    FILE *arquivoBass,
    jump *tabelaJumps,
    int totalJumps,
    int tamanhoArquivo
) {
    uint8_t *conteudo = malloc(tamanhoArquivo*sizeof(uint8_t));
    char linha[100];
    int indexConteudo = 4;
    conteudo[0] = 'b';
    conteudo[1] = 'v';
    conteudo[2] = 'm';
    conteudo[3] = 0x01;

    while(fgets(linha, 100, arquivoBass)) {
        processar_linha_assembly(linha, tabelaJumps, totalJumps, conteudo, &indexConteudo);
    }

    return conteudo;
}

uint8_t* processar_arquivo_assembly(char* nomeArquivoBass, int* tamanhoArquivo) {
    FILE *arquivoBass = fopen(nomeArquivoBass, "r");

    if (arquivoBass == NULL) {
        printf("Não foi possível abrir o arquivo: %s!\n", nomeArquivoBass);
        return NULL;
    }
    int totalJumps = 0;
    (*tamanhoArquivo) = 4;
    totalizar_instrucoes(arquivoBass, &totalJumps, tamanhoArquivo);

    rewind(arquivoBass);
    jump *tabelaJumps = montar_tabela_jumps(arquivoBass, totalJumps);

    rewind(arquivoBass);
    uint8_t *conteudo =  gerar_conteudo_binario(arquivoBass, tabelaJumps, totalJumps, *tamanhoArquivo);

    fclose(arquivoBass);

    for (int i = 0; i < totalJumps; i++) {
        free(tabelaJumps[i].label);
    }
    free(tabelaJumps);

    return conteudo;
}

void escrever_arquivo_binario(char* nomeArquivoBass, uint8_t* conteudo, int tamanhoArquivo) {
    int comprimentoNomeArquivo = strlen(nomeArquivoBass);
    char* nomeArquivoBbvm = malloc(comprimentoNomeArquivo);
    strcpy(nomeArquivoBbvm, nomeArquivoBass);
    strcpy(&nomeArquivoBbvm[comprimentoNomeArquivo-4], "bbvm");

    FILE *arquivoBbvm = fopen(nomeArquivoBbvm, "w+");

    if (arquivoBbvm == NULL) {
        printf("Erro ao salvar arquivo binário");
        return;
    }

    fwrite(conteudo, tamanhoArquivo, 1, arquivoBbvm);

    fclose(arquivoBbvm);
    free(nomeArquivoBbvm);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("É necessário especificar o arquivo de assembly!\n");
        return 0;
    }

    int tamanhoArquivo = 0;
    uint8_t* conteudo = processar_arquivo_assembly(argv[1], &tamanhoArquivo);
    escrever_arquivo_binario(argv[1], conteudo, tamanhoArquivo);

    free(conteudo);
}
