#include <stdio.h>
#include <stdint.h>

#define MAX_TAM_PILHA 64
#define MAX_TAM_PROGRAMA 64 * 8
#define MAX_TAM_MEMORIA 64 * 8

#define INST_NOP 0x00
#define INST_HALT 0x01

typedef struct bvm {
    uint64_t pc;

    uint64_t pilha[MAX_TAM_PILHA];
    uint64_t tam_pilha;

    uint8_t programa[MAX_TAM_PROGRAMA];
    uint64_t tam_programa;

    uint8_t memoria[MAX_TAM_MEMORIA];
    uint8_t halt;
} bvm;

void processar_instrucoes(bvm *vm) {
    uint8_t inst = vm->programa[vm->pc];
    switch (inst) {
        case INST_NOP:
            break;
        case INST_HALT:
            vm->halt = 1;
            break;
        default:
            printf("0x%02x\n", inst);
            break;

    }
    vm->pc++;
}

int main() {
    bvm vm = {
        .pc = 1,
        .pilha = {0},
        .tam_pilha = 0,
        .programa = {0x00, 0x02, 0x03, 0x04, 0x05, 0x01},
        .tam_programa = 3,
        .memoria = {0},
        .halt = 0
    };

    while (!vm.halt) {
        processar_instrucoes(&vm);
    }
}
