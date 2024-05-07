#include <stdio.h>
#include "bvm.h"
#include "instrucoes.h"

void operacao_binaria_u(bvm *vm, char operador) {
    uint64_t operando2 = vm->pilha[--vm->tam_pilha];
    uint64_t operando1 = vm->pilha[--vm->tam_pilha];

    switch (operador) {
        case '+':
            vm->pilha[vm->tam_pilha++] = (operando1 + operando2);
            break;
        case '-':
            vm->pilha[vm->tam_pilha++] = (operando1 - operando2);
            break;
        case '*':
            vm->pilha[vm->tam_pilha++] = (operando1 * operando2);
            break;
        case '/':
            vm->pilha[vm->tam_pilha++] = (operando1 / operando2);
            break;
        case '%':
            vm->pilha[vm->tam_pilha++] = (operando1 % operando2);
            break;
    }
}

void operacao_binaria_i(bvm *vm, char operador) {
    int64_t operando2 = vm->pilha[--vm->tam_pilha];
    int64_t operando1 = vm->pilha[--vm->tam_pilha];

    switch (operador) {
        case '+':
            vm->pilha[vm->tam_pilha++] = (operando1 + operando2);
            break;
        case '-':
            vm->pilha[vm->tam_pilha++] = (operando1 - operando2);
            break;
        case '*':
            vm->pilha[vm->tam_pilha++] = (operando1 * operando2);
            break;
        case '/':
            vm->pilha[vm->tam_pilha++] = (operando1 / operando2);
            break;
        case '%':
            vm->pilha[vm->tam_pilha++] = (operando1 % operando2);
            break;
    }
}

void operacao_binaria_f(bvm *vm, char operador) {
    double operando2 = vm->pilha[--vm->tam_pilha];
    double operando1 = vm->pilha[--vm->tam_pilha];

    switch (operador) {
        case '+':
            vm->pilha[vm->tam_pilha++] = (operando1 + operando2);
            break;
        case '-':
            vm->pilha[vm->tam_pilha++] = (operando1 - operando2);
            break;
        case '*':
            vm->pilha[vm->tam_pilha++] = (operando1 * operando2);
            break;
        case '/':
            vm->pilha[vm->tam_pilha++] = (operando1 / operando2);
            break;
    }
}

void processar_instrucoes(bvm *vm) {
    uint8_t inst = vm->programa[vm->pc++];
    uint64_t imediato = 0;

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
            operacao_binaria_u(vm, '+');
            break;
        case INST_SUBU:
            operacao_binaria_u(vm, '-');
            break;
        case INST_MULU:
            operacao_binaria_u(vm, '*');
            break;
        case INST_DIVU:
            operacao_binaria_u(vm, '/');
            break;
        case INST_REMU:
            operacao_binaria_u(vm, '%');
            break;

        case INST_ADD:
            operacao_binaria_i(vm, '+');
            break;
        case INST_SUB:
            operacao_binaria_i(vm, '-');
            break;
        case INST_MUL:
            operacao_binaria_i(vm, '*');
            break;
        case INST_DIV:
            operacao_binaria_i(vm, '/');
            break;
        case INST_REM:
            operacao_binaria_i(vm, '%');
            break;

        case INST_ADDF:
            operacao_binaria_f(vm, '+');
            break;
        case INST_SUBF:
            operacao_binaria_f(vm, '-');
            break;
        case INST_MULF:
            operacao_binaria_f(vm, '*');
            break;
        case INST_DIVF:
            operacao_binaria_f(vm, '/');
            break;

        default:
            printf("Instrução desconhecida: 0x%02x\n", inst);
            break;
    }

    printf("Dump Pilha:\n");
    for(uint64_t i = 0; i < vm->tam_pilha; i++) {
        printf("%016lu -> 0x%016lx\n", i, vm->pilha[i]);
    }
}
