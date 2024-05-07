#ifndef BVM_H
#define BVM_H

#include <stdint.h>

#define MAX_TAM_PILHA 64
#define MAX_TAM_PROGRAMA 64 * 8
#define MAX_TAM_MEMORIA 64 * 8

typedef struct bvm {
    uint64_t pc;

    uint64_t pilha[MAX_TAM_PILHA];
    uint64_t tam_pilha;

    uint8_t programa[MAX_TAM_PROGRAMA];
    uint64_t tam_programa;

    uint8_t memoria[MAX_TAM_MEMORIA];
    uint8_t halt;
} bvm;

void processar_instrucoes(bvm *vm);

#endif /* BVM_H */
