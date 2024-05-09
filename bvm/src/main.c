#include <stdio.h>
#include <string.h>
#include "bvm.h"
#include "instrucoes.h"

#define X_7_0 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#define X_6_0 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#define X_5_0 0x00, 0x00, 0x00, 0x00, 0x00
#define X_4_0 0x00, 0x00, 0x00, 0x00
#define TESTE_PILHA 0x1F, 0x00, 0xFF, 0xEE, 0xDD, 0x20, 0xCC, 0x88

void ler_programa_do_arquivo_binario(char* nomeArquivoBbvm, bvm *vm) {
    FILE *arquivoBbvm = fopen(nomeArquivoBbvm, "r");
    uint8_t buffer[MAX_TAM_PROGRAMA + 4];

    if (arquivoBbvm == NULL) {
        printf("Não foi possível abrir o arquivo: %s!\n", nomeArquivoBbvm);
        return;
    }

    fread(buffer, sizeof(buffer), 1, arquivoBbvm);
    if (!(strncmp((char*)buffer, "bvm", 3) == 0 && buffer[3] == 1)) {
        printf("Programa inválido\n");
        return;
    }

    for (int i = 0; i < MAX_TAM_PROGRAMA; i++) {
        vm->programa[i] = buffer[i+4];
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
        .programa = {
            INST_HALT
        },
        .tam_programa = 3,
        .memoria = {0},
        .halt = 0
    };

    ler_programa_do_arquivo_binario(argv[1], &vm);

    while (!vm.halt || vm.pc >= sizeof(vm.programa)) {
        processar_instrucoes(&vm);
    }
}
