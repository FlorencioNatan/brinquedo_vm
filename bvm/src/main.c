#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bvm.h"
#include "instrucoes.h"

#define CINCO_MB 5000000
#define POSICAO_TAMANHO_ARQUIVO 4
#define POSICAO_TAMANHO_PROGRAMA 8
#define TAMANHO_CABECALHO_BINARIO 12

uint64_t ler_uint64_t(uint8_t *buffer, int indiceBuffer) {
    uint64_t tamanhoPrograma = 0;
    for (int i = 64; i >= 8;i -= 8) {
        tamanhoPrograma |= ((uint64_t) buffer[indiceBuffer++]) << (64 - i);
    }
    return tamanhoPrograma;
}

uint32_t ler_uint32_t(uint8_t *buffer, int indiceBuffer) {
    uint32_t tamanhoPrograma = 0;
    for (int i = 32; i >= 8;i -= 8) {
        tamanhoPrograma |= ((uint32_t) buffer[indiceBuffer++]) << (32 - i);
    }
    return tamanhoPrograma;
}

uint16_t ler_uint16_t(uint8_t *buffer, int indiceBuffer) {
    uint16_t tamanhoPrograma = 0;
    for (int i = 16; i >= 8;i -= 8) {
        tamanhoPrograma |= ((uint16_t) buffer[indiceBuffer++]) << (16 - i);
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

    uint32_t tamanhoPrograma = ler_uint32_t(buffer, POSICAO_TAMANHO_PROGRAMA);
    vm->tam_programa = tamanhoPrograma;
    vm->programa = malloc(sizeof(uint8_t)*tamanhoPrograma);
    for (uint32_t i = 0; i < tamanhoPrograma; i++) {
        vm->programa[i] = buffer[i + TAMANHO_CABECALHO_BINARIO];
    }

    uint32_t tamanhoArquivo = ler_uint32_t(buffer, POSICAO_TAMANHO_ARQUIVO);
    uint32_t inicioSecaoData = TAMANHO_CABECALHO_BINARIO + tamanhoPrograma;

    if (tamanhoArquivo == inicioSecaoData) {
        return;
    }

    uint32_t i = inicioSecaoData;
    do {
        uint64_t endereco = ler_uint64_t(buffer, i);
        i += 8;
        uint8_t tipo = buffer[i++];
        uint64_t tamanho = ler_uint64_t(buffer, i);
        i += 8;

        for (uint32_t j = 0; j < tamanho; j++) {
            if (tipo == TIPO_DADO_64_BITS) {
                uint64_t valor = ler_uint64_t(buffer, i);
                insere_uint64_t_na_memoria(vm, endereco, valor);
                endereco += 8;
                i += 8;
            } else if(tipo == TIPO_DADO_32_BITS) {
                uint32_t valor = ler_uint32_t(buffer, i);
                insere_uint32_t_na_memoria(vm, endereco, valor);
                endereco += 4;
                i += 4;
            } else if(tipo == TIPO_DADO_16_BITS) {
                uint16_t valor = ler_uint16_t(buffer, i);
                insere_uint16_t_na_memoria(vm, endereco, valor);
                endereco += 2;
                i += 2;
            } else {
                uint8_t valor = buffer[i];
                vm->memoria[endereco] = valor;
                endereco += 1;
                i += 1;
            }
        }
    } while (i < tamanhoArquivo);
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
        int resultadoExecucao = processar_instrucoes(&vm);

        switch (resultadoExecucao) {
                break;
            case EXEC_ERRO_INSTRUCAO_INVALIDA:
                vm.halt = 1;
                printf("Instrução desconhecida: %ld\n", vm.pc-1);
                break;
            case EXEC_ERRO_STACK_UNDERFLOW:
                vm.halt = 1;
                printf("Stack underflow na instrução: %ld\n", vm.pc-1);
                break;
            case EXEC_ERRO_STACK_OVERFLOW:
                vm.halt = 1;
                printf("Stack overflow na instrução: %ld\n", vm.pc-1);
                break;
            case EXEC_ERRO_DIVISAO_POR_ZERO:
                vm.halt = 1;
                printf("Divisão por zero na instrução: %ld\n", vm.pc-1);
                break;
            case EXEC_ERRO_ACESSO_INVALIDO_A_MEMORIA:
                vm.halt = 1;
                printf("Acesso inválido à memória na instrução: %ld\n", vm.pc-1);
                break;
            case EXEC_ERRO_EXTENSAO_NAO_EXISTE:
                vm.halt = 1;
                printf("A extensão não existe: %ld\n", vm.pc-1);
                break;
            case EXEC_ERRO_TAMANHO_MEMORIA_PEQUENO_PARA_EXTENSAO:
                vm.halt = 1;
                printf("Tamanho de memória muito pequeno para a extensão: %ld\n", vm.pc-1);
                break;
        }
    }
}
