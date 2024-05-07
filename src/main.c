#include <stdio.h>
#include <stdint.h>

#define X_7_0 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#define X_6_0 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#define X_5_0 0x00, 0x00, 0x00, 0x00, 0x00
#define X_4_0 0x00, 0x00, 0x00, 0x00
#define TESTE_PILHA 0x1F, 0x00, 0xFF, 0xEE, 0xDD, 0x20, 0xCC, 0x88


#define MAX_TAM_PILHA 64
#define MAX_TAM_PROGRAMA 64 * 8
#define MAX_TAM_MEMORIA 64 * 8

#define INST_NOP  0x00
#define INST_HALT 0x01
#define INST_PUSH 0x02
#define INST_POP  0x03

#define INST_ADDU 0x04
#define INST_SUBU 0x05
#define INST_MULU 0x06
#define INST_DIVU 0x07
#define INST_REMU 0x08

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
    uint8_t inst = vm->programa[vm->pc++];
    uint64_t imediato = 0;
    uint64_t operando1;
    uint64_t operando2;

    printf("Inst: 0x%02x\n", inst);
    switch (inst) {
        case INST_NOP:
            break;
        case INST_HALT:
            vm->halt = 1;
            break;
        case INST_PUSH:
            for (uint64_t i = 8; i <= 64;i += 8) {
                imediato |= ((uint64_t) vm->programa[vm->pc++]) << (64 - i);
            }
            vm->pilha[vm->tam_pilha++] = imediato;
            break;
        case INST_POP:
            imediato =  vm->pilha[vm->tam_pilha - 1];
            vm->tam_pilha--;
            break;
        case INST_ADDU:
            ;
            operando2 = vm->pilha[--vm->tam_pilha];
            operando1 = vm->pilha[--vm->tam_pilha];
            vm->pilha[vm->tam_pilha++] = (operando1 + operando2);
            break;
        case INST_SUBU:
            ;
            operando2 = vm->pilha[--vm->tam_pilha];
            operando1 = vm->pilha[--vm->tam_pilha];
            vm->pilha[vm->tam_pilha++] = (operando1 - operando2);
            break;
        case INST_MULU:
            ;
            operando2 = vm->pilha[--vm->tam_pilha];
            operando1 = vm->pilha[--vm->tam_pilha];
            vm->pilha[vm->tam_pilha++] = (operando1 * operando2);
            break;
        case INST_DIVU:
            ;
            operando2 = vm->pilha[--vm->tam_pilha];
            operando1 = vm->pilha[--vm->tam_pilha];
            vm->pilha[vm->tam_pilha++] = (operando1 / operando2);
            break;
        case INST_REMU:
            ;
            operando2 = vm->pilha[--vm->tam_pilha];
            operando1 = vm->pilha[--vm->tam_pilha];
            vm->pilha[vm->tam_pilha++] = (operando1 % operando2);
            break;
        default:
            printf("Instrução desconhecida: 0x%02x\n", inst);
            break;

    }
    printf("Pilha:\n");
    for(uint64_t i = 0; i < vm->tam_pilha; i++) {
        printf("%016lu -> 0x%016lx\n", i, vm->pilha[i]);
    }
}

int main() {
    bvm vm = {
        .pc = 0,
        .pilha = {0},
        .tam_pilha = 0,
        .programa = {
            INST_PUSH, TESTE_PILHA,
            INST_POP,
            INST_PUSH, X_7_0, 0xF4,
            INST_PUSH, X_7_0, 0x24,
            INST_REMU,
            INST_HALT
        },
        .tam_programa = 3,
        .memoria = {0},
        .halt = 0
    };

    while (!vm.halt || vm.pc >= sizeof(vm.programa)) {
        processar_instrucoes(&vm);
    }
}
