#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bvm.h"
#include "instrucoes.h"

#define CINCO_MB 5000000
#define X_7_0 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#define X_6_0 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#define X_5_0 0x00, 0x00, 0x00, 0x00, 0x00
#define X_4_0 0x00, 0x00, 0x00, 0x00
#define TESTE_PILHA 0x1F, 0x00, 0xFF, 0xEE, 0xDD, 0x20, 0xCC, 0x88

int ler_tamanho_programa(uint8_t *buffer) {
    int tamanhoPrograma = 0;
    int indiceBuffer = 4;
    for (int i = 32; i >= 8;i -= 8) {
        tamanhoPrograma |= ((uint32_t) buffer[indiceBuffer++]) << (64 - i);
    }
    return tamanhoPrograma;
}

void ler_programa_do_arquivo_binario(char* nomeArquivoBbvm, bvm *vm) {
    FILE *arquivoBbvm = fopen(nomeArquivoBbvm, "r");
    fseek(arquivoBbvm, 0, SEEK_END);
    int tamanhoArquivoBytes = ftell(arquivoBbvm);
    if (tamanhoArquivoBytes > CINCO_MB) {
        printf("O tamanho máximo do arquivo de assembly suportado é 5MB.");
        vm->programa = malloc(sizeof(uint8_t));
        vm->programa[0] = INST_HALT;
        return;
    }
    fseek(arquivoBbvm, 0, SEEK_SET);

    uint8_t buffer[tamanhoArquivoBytes + 1];

    if (arquivoBbvm == NULL) {
        printf("Não foi possível abrir o arquivo: %s!\n", nomeArquivoBbvm);
        return;
    }

    fread(buffer, sizeof(buffer), 1, arquivoBbvm);
    if (!(strncmp((char*)buffer, "bvm", 3) == 0 && buffer[3] == 1)) {
        printf("Programa inválido\n");
        return;
    }

    int tamanhoPrograma = ler_tamanho_programa(buffer);
    vm->tam_programa = tamanhoPrograma;
    vm->programa = malloc(sizeof(uint8_t)*tamanhoPrograma);
    for (int i = 0; i < tamanhoPrograma; i++) {
        vm->programa[i] = buffer[i+8];
    }

    fclose(arquivoBbvm);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("É necessário especificar o arquivo de assembly!\n");
        return 0;
    }

    bvm vm = {
        .pc = 0,
        .pilha = {0},
        .tam_pilha = 0,
        .tam_programa = 3,
        .memoria = {0},
        .halt = 0
    };

    ler_programa_do_arquivo_binario(argv[1], &vm);

    while (!vm.halt && vm.pc <= vm.tam_programa) {
        processar_instrucoes(&vm);
    }
}
