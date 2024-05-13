#ifndef BVM_H
#define BVM_H

#include <stdint.h>

#define MAX_TAM_PILHA 64
#define MAX_TAM_MEMORIA 5000000 // 5MB

typedef struct bvm {
    uint64_t pc;

    uint64_t pilha[MAX_TAM_PILHA];
    uint64_t tam_pilha;

    uint8_t *programa;
    uint64_t tam_programa;

    uint8_t memoria[MAX_TAM_MEMORIA];
    uint8_t halt;
} bvm;

void processar_instrucoes(bvm *vm);

#endif /* BVM_H */
